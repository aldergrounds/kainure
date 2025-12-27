/* ============================================================================ *
 * Kainure - Node.js Framework for SA-MP (San Andreas Multiplayer)              *
 * ================================= About ==================================== *
 *                                                                              *
 * Kainure embeds Node.js runtime into SA-MP servers, enabling developers       *
 * to write gamemodes using modern JavaScript/TypeScript with full access       *
 * to the Node.js ecosystem, async/await, npm packages, and native SA-MP        *
 * functions through automatic bindings.                                        *
 *                                                                              *
 * =============================== Copyright ================================== *
 *                                                                              *
 * Copyright (c) 2025, AlderGrounds                                             *
 * All rights reserved.                                                         *
 *                                                                              *
 * Repository: https://github.com/aldergrounds/kainure                          *
 *                                                                              *
 * ================================ License =================================== *
 *                                                                              *
 * Licensed under the Apache License, Version 2.0 (the "License");              *
 * you may not use this file except in compliance with the License.             *
 * You may obtain a copy of the License at:                                     *
 *                                                                              *
 *     http://www.apache.org/licenses/LICENSE-2.0                               *
 *                                                                              *
 * Unless required by applicable law or agreed to in writing, software          *
 * distributed under the License is distributed on an "AS IS" BASIS,            *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.     *
 * See the License for the specific language governing permissions and          *
 * limitations under the License.                                               *
 *                                                                              *
 * ============================================================================ */

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdint>
#include <exception>
#include <string_view>
//
#include "code_preprocessor.hpp"
#include "logger.hpp"

namespace {
    constexpr std::string_view NATIVE_KEYWORD = "Native";
    constexpr std::string_view CALL_PUBLIC_KEYWORD = "Call_Public";
    constexpr std::string_view FLOAT_WRAPPER = "Float";

    constexpr std::array<char, 6> VALID_NUM_CONTEXT = { ',', '(', '=', '[', ':', ' ' };
    
    enum class Parse_State : uint8_t {
        Normal,
        In_Native_Call,
        In_Call_Public,
        In_String,
        In_Line_Comment,
        In_Block_Comment
    };

    struct Keyword_Match {
        bool matched = false;
        size_t length = 0;
    };

    inline bool Is_Identifier_Boundary(char c) noexcept {
        const unsigned char uc = static_cast<unsigned char>(c);

        return !((uc >= 'a' && uc <= 'z') || (uc >= 'A' && uc <= 'Z') || (uc >= '0' && uc <= '9') || c == '_' || c == '$');
    }

    inline bool Is_Word_Start(const std::string& code, size_t pos) noexcept {
        return pos == 0 || Is_Identifier_Boundary(code[pos - 1]);
    }

    inline bool Is_Word_End(const std::string& code, size_t pos, size_t keyword_len) noexcept {
        size_t end_pos = pos + keyword_len;

        return end_pos >= code.length() || Is_Identifier_Boundary(code[end_pos]);
    }

    inline Keyword_Match Match_Keyword(const std::string& code, size_t pos, std::string_view keyword) noexcept {
        Keyword_Match result;

        if (pos + keyword.length() > code.length())
            return result;

        if (!Is_Word_Start(code, pos))
            return result;

        if (code.compare(pos, keyword.length(), keyword) == 0) {
            if (Is_Word_End(code, pos, keyword.length())) {
                result.matched = true;
                result.length = keyword.length();
            }
        }

        return result;
    }

    inline bool Is_Identifier_Start(char c) noexcept {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '$';
    }

    inline bool Is_Identifier_Char(char c) noexcept {
        return Is_Identifier_Start(c) || (c >= '0' && c <= '9');
    }

    struct Number_Info {
        size_t start = 0;
        size_t end = 0;
        bool has_sign = false;
        bool is_valid_float = false;
    };

    inline Number_Info Parse_Number(const std::string& code, size_t start_pos, size_t chunk_start) noexcept {
        Number_Info info;
        info.start = start_pos;

        const size_t len = code.length();
        
        if (start_pos >= len)
            return info;

        size_t pos = start_pos;

        while (pos < len && std::isdigit(static_cast<unsigned char>(code[pos])))
            ++pos;

        if (pos >= len || code[pos] != '.')
            return info;

        ++pos;

        size_t decimal_start = pos;
        bool only_zeros = true;
        bool has_decimals = false;

        while (pos < len && std::isdigit(static_cast<unsigned char>(code[pos]))) {
            if (code[pos] != '0')
                only_zeros = false;

            has_decimals = true;
            ++pos;
        }

        if (!has_decimals || !only_zeros)
            return info;

        if (pos < len && !Is_Identifier_Boundary(code[pos]))
            return info;

        size_t check_pos = start_pos;

        if (check_pos > chunk_start) {
            size_t scan = check_pos - 1;

            while (scan > chunk_start && std::isspace(static_cast<unsigned char>(code[scan])))
                --scan;

            if (code[scan] == '-' || code[scan] == '+') {
                size_t before_sign = scan;

                if (before_sign > chunk_start) {
                    --before_sign;

                    while (before_sign > chunk_start && std::isspace(static_cast<unsigned char>(code[before_sign])))
                        --before_sign;

                    char ctx = code[before_sign];

                    if (std::find(VALID_NUM_CONTEXT.begin(), VALID_NUM_CONTEXT.end(), ctx) != VALID_NUM_CONTEXT.end()) {
                        info.start = scan;
                        info.has_sign = true;
                    }
                }
            }
        }

        info.end = pos;
        info.is_valid_float = true;

        return info;
    }

    inline bool Is_Already_Wrapped(const std::string& code, size_t num_start) noexcept {
        if (num_start == 0)
            return false;

        size_t pos = num_start - 1;

        while (pos > 0 && std::isspace(static_cast<unsigned char>(code[pos])))
            --pos;

        if (code[pos] != '(')
            return false;

        if (pos == 0)
            return false;

        --pos;

        while (pos > 0 && std::isspace(static_cast<unsigned char>(code[pos])))
            --pos;

        if (pos < FLOAT_WRAPPER.length() - 1)
            return false;

        size_t float_start = pos - FLOAT_WRAPPER.length() + 1;

        if (code.compare(float_start, FLOAT_WRAPPER.length(), FLOAT_WRAPPER) == 0)
            return float_start == 0 || Is_Identifier_Boundary(code[float_start - 1]);

        return false;
    }
}

std::string Code_Preprocessor::Transform_Native_Calls(const std::string& code) {
    const size_t len = code.length();

    if (len == 0)
        return code;

    if (len < 8)
        return code;

    if (code.find("Native.") == std::string::npos && code.find("Call_Public.") == std::string::npos)
        return code;

    try {
        std::string result;
        result.reserve(len + (len / 10));

        Parse_State state = Parse_State::Normal;
        char quote_char = '\0';
        int depth = 0;
        size_t chunk_start = 0;

        for (size_t i = 0; i < len; ++i) {
            const char c = code[i];
            const char next = (i + 1 < len) ? code[i + 1] : '\0';

            switch (state) {
                case Parse_State::In_Line_Comment:
                    if (c == '\n')
                        state = Parse_State::Normal;

                    continue;
                case Parse_State::In_Block_Comment:
                    if (c == '*' && next == '/') {
                        state = Parse_State::Normal;
                        ++i;
                    }

                    continue;
                case Parse_State::In_String:
                    if (c == '\\') {
                        if (i + 1 < len)
                            ++i;
                    }
                    else if (c == quote_char) {
                        state = Parse_State::Normal;
                        quote_char = '\0';
                    }

                    continue;
                default:
                    break;
            }

            if (state == Parse_State::Normal) {
                if (c == '/' && next == '/') {
                    state = Parse_State::In_Line_Comment;
                    ++i;

                    continue;
                }

                if (c == '/' && next == '*') {
                    state = Parse_State::In_Block_Comment;
                    ++i;

                    continue;
                }

                if (c == '"' || c == '\'' || c == '`') {
                    state = Parse_State::In_String;
                    quote_char = c;

                    continue;
                }
            }

            if (state == Parse_State::Normal && depth == 0) {
                if (c == 'N') {
                    auto match = Match_Keyword(code, i, NATIVE_KEYWORD);

                    if (match.matched) {
                        size_t scan = i + match.length;

                        while (scan < len && std::isspace(static_cast<unsigned char>(code[scan])))
                            ++scan;

                        if (scan < len && code[scan] == '.') {
                            ++scan;

                            while (scan < len && std::isspace(static_cast<unsigned char>(code[scan])))
                                ++scan;

                            if (scan < len && Is_Identifier_Start(code[scan])) {
                                while (scan < len && Is_Identifier_Char(code[scan]))
                                    ++scan;

                                while (scan < len && std::isspace(static_cast<unsigned char>(code[scan])))
                                    ++scan;

                                if (scan < len && code[scan] == '(') {
                                    state = Parse_State::In_Native_Call;

                                    depth = 1;
                                    i = scan;

                                    continue;
                                }
                            }
                        }
                    }
                }
                else if (c == 'C') {
                    auto match = Match_Keyword(code, i, CALL_PUBLIC_KEYWORD);

                    if (match.matched) {
                        size_t scan = i + match.length;

                        while (scan < len && std::isspace(static_cast<unsigned char>(code[scan])))
                            ++scan;

                        if (scan < len && code[scan] == '.') {
                            ++scan;

                            while (scan < len && std::isspace(static_cast<unsigned char>(code[scan])))
                                ++scan;

                            if (scan < len && Is_Identifier_Start(code[scan])) {
                                while (scan < len && Is_Identifier_Char(code[scan]))
                                    ++scan;

                                while (scan < len && std::isspace(static_cast<unsigned char>(code[scan])))
                                    ++scan;

                                if (scan < len && code[scan] == '(') {
                                    state = Parse_State::In_Call_Public;

                                    depth = 1;
                                    i = scan;

                                    continue;
                                }
                            }
                        }
                    }
                }
            }

            if ((state == Parse_State::In_Native_Call || state == Parse_State::In_Call_Public) && depth > 0) {
                if (c == '(')
                    ++depth;
                else if (c == ')') {
                    --depth;

                    if (depth == 0)
                        state = Parse_State::Normal;
                }
                else if (std::isdigit(static_cast<unsigned char>(c))) {
                    bool is_num_start = (i == 0) || !Is_Identifier_Char(code[i - 1]);

                    if (is_num_start) {
                        auto num_info = Parse_Number(code, i, chunk_start);

                        if (num_info.is_valid_float) {
                            if (!Is_Already_Wrapped(code, num_info.start)) {
                                result.append(code, chunk_start, num_info.start - chunk_start);
                                result.append("Float(");
                                result.append(code, num_info.start, num_info.end - num_info.start);
                                result.append(")");

                                chunk_start = num_info.end;
                                i = num_info.end - 1;
                            }
                        }
                    }
                }
            }
        }

        if (chunk_start < len)
            result.append(code, chunk_start, len - chunk_start);

        return result;
    }
    catch (const std::bad_alloc& e) {
        return (Logger::Log(Log_Level::ERROR_s, "Memory allocation failed in code preprocessor: '%s'.", e.what()), code);
    }
    catch (const std::exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "Unexpected error in code preprocessor: '%s'.", e.what()), code);
    }
}
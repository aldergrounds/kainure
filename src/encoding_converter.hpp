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

#pragma once

#include <string>

class Encoding_Converter {
    public:
        static Encoding_Converter& Instance();

        void Initialize(bool enabled, const std::string& target);

        bool Is_Enabled() const {
            return encoding_enabled_;
        }

        unsigned int Get_Target_Codepage() const {
            return target_codepage_;
        }

        const std::string& Get_Target_Encoding() const {
            return target_encoding_;
        }

        std::string UTF8_To_Target(const std::string& utf8_str) const;
        std::string Target_To_UTF8(const std::string& target_str) const;

        bool UTF8_To_Target(const std::string& utf8_str, char* out_buffer, size_t buffer_size, size_t& out_length) const;
        bool Target_To_UTF8(const char* target_str, size_t target_len, std::string& out_utf8) const;

    private:
        Encoding_Converter() = default;
        ~Encoding_Converter() = default;

        Encoding_Converter(const Encoding_Converter&) = delete;
        Encoding_Converter& operator=(const Encoding_Converter&) = delete;

        bool encoding_enabled_ = false;
        unsigned int target_codepage_ = 65001;
        std::string target_encoding_;
        bool is_windows_mode_ = true;

#if defined(_WIN32)
        std::wstring UTF8_To_UTF16(const std::string& utf8_str) const;
        std::string UTF16_To_UTF8(const std::wstring& utf16_str) const;
        std::string UTF16_To_CodePage(const std::wstring& utf16_str, unsigned int codepage) const;
        std::wstring CodePage_To_UTF16(const std::string& str, unsigned int codepage) const;
#elif defined(__linux__)
        std::string Iconv_Convert(const std::string& input, const std::string& from_encoding, const std::string& to_encoding) const;
#endif
};
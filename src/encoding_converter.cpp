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

#include <cstring>
#include <cctype>
#include <vector>
//
#include "encoding_converter.hpp"
#include "constants.hpp"
#include "logger.hpp"
#include "exceptions.hpp"

#if defined(_WIN32)
    #include <windows.h>
#elif defined(__linux__)
    #include <iconv.h>
    #include <errno.h>
#endif

namespace {
    bool Is_Only_Digits(const std::string& str) {
        if (str.empty())
            return false;

        for (char c : str) {
            if (!std::isdigit(static_cast<unsigned char>(c)))
                return false;
        }

        return true;
    }
}

Encoding_Converter& Encoding_Converter::Instance() {
    static Encoding_Converter instance;

    return instance;
}

void Encoding_Converter::Initialize(bool enabled, const std::string& target) {
    encoding_enabled_ = enabled;

    if (!encoding_enabled_) {
        Logger::Log(Log_Level::INFO, "Encoding conversion disabled. Using UTF-8 passthrough.");

        target_codepage_ = 65001;
        target_encoding_ = "UTF-8";
        is_windows_mode_ = true;

        return;
    }

    is_windows_mode_ = Is_Only_Digits(target);

    if (is_windows_mode_) {
        try {
            target_codepage_ = static_cast<unsigned int>(std::stoul(target));
        }
        catch (const std::exception& e) {
            Logger::Log(Log_Level::WARNING, "Invalid numeric codepage '%s': %s. Falling back to UTF-8 (65001).", target.c_str(), e.what());

            target_codepage_ = 65001;
            encoding_enabled_ = false;
            target_encoding_ = "UTF-8";
            is_windows_mode_ = true;

            return;
        }

#if defined(_WIN32)
        if (!IsValidCodePage(target_codepage_)) {
            Logger::Log(Log_Level::WARNING, "Invalid code page '%u' specified. Falling back to UTF-8 (65001).", target_codepage_);

            target_codepage_ = 65001;
            encoding_enabled_ = false;
        }
        else
            Logger::Log(Log_Level::INFO, "Encoding conversion enabled. Target code page: '%u'.", target_codepage_);
#elif defined(__linux__)
        Logger::Log(Log_Level::WARNING, "Numeric codepage '%u' specified but running on Linux. Please use encoding name (e.g., 'WINDOWS-1251'). Falling back to UTF-8.", target_codepage_);

        target_codepage_ = 65001;
        target_encoding_ = "UTF-8";
        encoding_enabled_ = false;
#endif
    }
    else {
        target_encoding_ = target;

#if defined(_WIN32)
        Logger::Log(Log_Level::WARNING, "Encoding name '%s' specified but running on Windows. Please use numeric codepage (e.g., 1251). Falling back to UTF-8.", target.c_str());

        target_codepage_ = 65001;
        target_encoding_ = "UTF-8";
        encoding_enabled_ = false;
        is_windows_mode_ = true;
#elif defined(__linux__)
        iconv_t test_cd = iconv_open(target_encoding_.c_str(), "UTF-8");
        
        if (test_cd == (iconv_t)-1) {
            Logger::Log(Log_Level::WARNING, "Encoding '%s' not supported on this system. Falling back to UTF-8.", target_encoding_.c_str());

            target_encoding_ = "UTF-8";
            encoding_enabled_ = false;
        }
        else {
            iconv_close(test_cd);
            Logger::Log(Log_Level::INFO, "Encoding conversion enabled. Target encoding: '%s'.", target_encoding_.c_str());
        }
#endif
    }
}

std::string Encoding_Converter::UTF8_To_Target(const std::string& utf8_str) const {
    if (!encoding_enabled_ || utf8_str.empty())
        return utf8_str;

#if defined(_WIN32)
    if (target_codepage_ == 65001)
        return utf8_str;

    try {
        std::wstring utf16 = UTF8_To_UTF16(utf8_str);

        return UTF16_To_CodePage(utf16, target_codepage_);
    }
    catch (const Encoding_Exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "UTF8_To_Target conversion failed: '%s'. Returning original string.", e.what()), utf8_str);
    }
#elif defined(__linux__)
    if (target_encoding_ == "UTF-8")
        return utf8_str;

    try {
        return Iconv_Convert(utf8_str, "UTF-8", target_encoding_);
    }
    catch (const Encoding_Exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "UTF8_To_Target conversion failed: '%s'. Returning original string.", e.what()), utf8_str);
    }
#endif
}

std::string Encoding_Converter::Target_To_UTF8(const std::string& target_str) const {
    if (!encoding_enabled_ || target_str.empty())
        return target_str;

#if defined(_WIN32)
    if (target_codepage_ == 65001)
        return target_str;

    try {
        std::wstring utf16 = CodePage_To_UTF16(target_str, target_codepage_);
        
        return UTF16_To_UTF8(utf16);
    }
    catch (const Encoding_Exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "Target_To_UTF8 conversion failed: '%s'. Returning original string.", e.what()), target_str);
    }
#elif defined(__linux__)
    if (target_encoding_ == "UTF-8")
        return target_str;

    try {
        return Iconv_Convert(target_str, target_encoding_, "UTF-8");
    }
    catch (const Encoding_Exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "Target_To_UTF8 conversion failed: '%s'. Returning original string.", e.what()), target_str);
    }
#endif
}

bool Encoding_Converter::UTF8_To_Target(const std::string& utf8_str, char* out_buffer, size_t buffer_size, size_t& out_length) const {
    if (!out_buffer || buffer_size == 0)
        return false;

    std::string converted = UTF8_To_Target(utf8_str);
    out_length = converted.length();

    if (out_length >= buffer_size) {
        Logger::Log(Log_Level::WARNING, "Encoding conversion result too large for buffer ('%zu' >= '%zu'). Truncating.", out_length, buffer_size);

        out_length = buffer_size - 1;
    }

    memcpy(out_buffer, converted.c_str(), out_length);
    out_buffer[out_length] = '\0';

    return true;
}

bool Encoding_Converter::Target_To_UTF8(const char* target_str, size_t target_len, std::string& out_utf8) const {
    if (!target_str || target_len == 0)
        return false;

    std::string target(target_str, target_len);
    out_utf8 = Target_To_UTF8(target);

    return true;
}

#if defined(_WIN32)
std::wstring Encoding_Converter::UTF8_To_UTF16(const std::string& utf8_str) const {
    if (utf8_str.empty())
        return std::wstring();

    int required_size = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), static_cast<int>(utf8_str.length()), nullptr, 0);

    if (required_size <= 0)
        throw Encoding_Exception("'MultiByteToWideChar' failed (UTF8 to UTF16).");

    std::wstring utf16_str(required_size, 0);

    int result = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), static_cast<int>(utf8_str.length()), &utf16_str[0], required_size);

    if (result <= 0)
        throw Encoding_Exception("'MultiByteToWideChar' conversion failed.");

    return utf16_str;
}

std::string Encoding_Converter::UTF16_To_UTF8(const std::wstring& utf16_str) const {
    if (utf16_str.empty())
        return std::string();

    int required_size = WideCharToMultiByte(CP_UTF8, 0, utf16_str.c_str(), static_cast<int>(utf16_str.length()), nullptr, 0, nullptr, nullptr);

    if (required_size <= 0)
        throw Encoding_Exception("'WideCharToMultiByte' failed (UTF16 to UTF8).");

    std::string utf8_str(required_size, 0);

    int result = WideCharToMultiByte(CP_UTF8, 0, utf16_str.c_str(), static_cast<int>(utf16_str.length()), &utf8_str[0], required_size, nullptr, nullptr);

    if (result <= 0)
        throw Encoding_Exception("'WideCharToMultiByte' conversion failed.");

    return utf8_str;
}

std::string Encoding_Converter::UTF16_To_CodePage(const std::wstring& utf16_str, unsigned int codepage) const {
    if (utf16_str.empty())
        return std::string();

    int required_size = WideCharToMultiByte(codepage, 0, utf16_str.c_str(), static_cast<int>(utf16_str.length()), nullptr, 0, nullptr, nullptr);

    if (required_size <= 0)
        throw Encoding_Exception("'WideCharToMultiByte' failed (UTF16 to CodePage).");

    std::string target_str(required_size, 0);

    int result = WideCharToMultiByte(codepage, 0, utf16_str.c_str(), static_cast<int>(utf16_str.length()), &target_str[0], required_size, nullptr, nullptr);

    if (result <= 0)
        throw Encoding_Exception("'WideCharToMultiByte' conversion to target codepage failed.");

    return target_str;
}

std::wstring Encoding_Converter::CodePage_To_UTF16(const std::string& str, unsigned int codepage) const {
    if (str.empty())
        return std::wstring();

    int required_size = MultiByteToWideChar(codepage, 0, str.c_str(), static_cast<int>(str.length()), nullptr, 0);

    if (required_size <= 0)
        throw Encoding_Exception("'MultiByteToWideChar' failed (CodePage to UTF16).");

    std::wstring utf16_str(required_size, 0);

    int result = MultiByteToWideChar(codepage, 0, str.c_str(), static_cast<int>(str.length()), &utf16_str[0], required_size);

    if (result <= 0)
        throw Encoding_Exception("'MultiByteToWideChar' conversion from codepage failed.");

    return utf16_str;
}
#elif defined(__linux__)
std::string Encoding_Converter::Iconv_Convert(const std::string& input, const std::string& from_encoding, const std::string& to_encoding) const {
    if (input.empty())
        return std::string();

    iconv_t cd = iconv_open(to_encoding.c_str(), from_encoding.c_str());
    
    if (cd == (iconv_t)-1)
        throw Encoding_Exception("'iconv_open' failed: " + std::string(strerror(errno)));

    size_t inbytesleft = input.size();
    size_t outbufsize = inbytesleft * 4 + 1;
    std::vector<char> outbuf(outbufsize);
    
    char* inbuf = const_cast<char*>(input.data());
    char* outptr = outbuf.data();
    size_t outbytesleft = outbufsize;

    size_t result = iconv(cd, &inbuf, &inbytesleft, &outptr, &outbytesleft);
    
    iconv_close(cd);

    if (result == (size_t)-1) {
        if (errno == E2BIG)
            throw Encoding_Exception("'iconv' output buffer too small.");
        else if (errno == EILSEQ)
            throw Encoding_Exception("'iconv' invalid multibyte sequence.");
        else if (errno == EINVAL)
            throw Encoding_Exception("'iconv' incomplete multibyte sequence.");
        else
            throw Encoding_Exception("'iconv' conversion failed: " + std::string(strerror(errno)));
    }

    size_t converted_size = outbufsize - outbytesleft;

    return std::string(outbuf.data(), converted_size);
}
#endif
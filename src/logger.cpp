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

#include <cstdarg>
#include <cstdio>
//
#include "samp-sdk/logger.hpp"
//
#include "constants.hpp"
#include "logger.hpp"

void Logger::Log(Log_Level level, const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    char buffer[Constants::LOG_BUFFER_SIZE];
    vsnprintf(buffer, sizeof(buffer), format, args);
    
    va_end(args);

    std::string message_content = Get_Level_Prefix(level) + " " + buffer;
    std::string full_message = "[Kainure]:" + message_content;

    Samp_SDK::Log("%s", full_message.c_str());
}

std::string Logger::Get_Level_Prefix(Log_Level level) {
    switch (level) {
        case Log_Level::INFO:
            return "[Info]";
        case Log_Level::ERROR_s:
            return "[Error]";
        case Log_Level::WARNING:
            return "[Warning]";
        default:
            return "[Unknown]";
    }
}
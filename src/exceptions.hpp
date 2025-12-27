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

#include <exception>
#include <string>

class Plugin_Exception : public std::exception {
    protected:
        std::string message_;

    public:
        explicit Plugin_Exception(const std::string& msg) : message_(msg) {}

        const char* what() const noexcept override {
            return message_.c_str();
        }
};

class Runtime_Exception : public Plugin_Exception {
    public:
        explicit Runtime_Exception(const std::string& msg) : Plugin_Exception("[Runtime_Exception] " + msg) {}
};

class File_Exception : public Plugin_Exception {
    public:
        explicit File_Exception(const std::string& msg) : Plugin_Exception("[File_Exception] " + msg) {}
};

class Script_Exception : public Plugin_Exception {
    public:
        explicit Script_Exception(const std::string& msg) : Plugin_Exception("[Script_Exception] " + msg) {}
};

class Initialization_Exception : public Plugin_Exception {
    public:
        explicit Initialization_Exception(const std::string& msg) : Plugin_Exception("[Initialization_Exception] " + msg) {}
};

class V8_Exception : public Plugin_Exception {
    public:
        explicit V8_Exception(const std::string& msg) : Plugin_Exception("[V8_Exception] " + msg) {}
};

class Config_Exception : public Plugin_Exception {
    public:
        explicit Config_Exception(const std::string& msg) : Plugin_Exception("[Config_Exception] " + msg) {}
};

class AMX_Exception : public Plugin_Exception {
    public:
        explicit AMX_Exception(const std::string& msg) : Plugin_Exception("[AMX_Exception] " + msg) {}
};

class TypeScript_Exception : public Plugin_Exception {
    public:
        explicit TypeScript_Exception(const std::string& msg) : Plugin_Exception("[TypeScript_Exception] " + msg) {}
};
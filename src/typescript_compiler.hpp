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
//
#include "node_inclusion.hpp"

class TypeScript_Compiler {
    public:
        static TypeScript_Compiler& Instance();

        bool Compile_Project(v8::Isolate* isolate, v8::Local<v8::Context> context);
        bool Is_TypeScript_File(const std::string& path) const;
        std::string Get_Output_Path(const std::string& ts_path) const;

    private:
        TypeScript_Compiler() = default;
        ~TypeScript_Compiler() = default;

        TypeScript_Compiler(const TypeScript_Compiler&) = delete;
        TypeScript_Compiler& operator=(const TypeScript_Compiler&) = delete;

        bool Check_TypeScript_Installation(v8::Isolate* isolate, v8::Local<v8::Context> context);
        bool Install_TypeScript_Automatically(v8::Isolate* isolate, v8::Local<v8::Context> context);
        bool Execute_TypeScript_Compiler(v8::Isolate* isolate, v8::Local<v8::Context> context);
};
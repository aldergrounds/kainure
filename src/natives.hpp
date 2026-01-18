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
#include <cstdint>
#include <vector>
#include <memory>
//
#include "sdk/amx/amx_defs.h" 
//
#include "node_inclusion.hpp"

struct Native_Binding_Data {
    AMX_NATIVE native_func = nullptr;
    uint32_t native_hash = 0;
    std::string native_name;
};

class Natives {
    public:
        static void Generate_Binding(v8::Isolate* isolate, v8::Local<v8::Object> target, const std::string& name, uint32_t hash);
        static void Set_Has_Hooks(bool has_hooks);
        static void Handler(const v8::FunctionCallbackInfo<v8::Value>& info);
        static void Clear_Bindings();

    private:
        static std::vector<std::unique_ptr<Native_Binding_Data>> bindings_storage_;
};
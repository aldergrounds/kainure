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

#include <functional>
#include <memory>
//
#include "samp-sdk/amx_defs.h"
#include "samp-sdk/amx_memory.hpp"
//
#include "node_inclusion.hpp"

class Type_Converter {
    public:
        struct Conversion_Result {
            cell value = 0;

            std::unique_ptr<Samp_SDK::Amx_Scoped_Memory> memory;
            std::function<void()> updater;

            bool Has_Updater() const {
                return updater != nullptr;
            }
        };

        static Conversion_Result To_Cell(v8::Isolate* isolate, v8::Local<v8::Context> context, v8::Local<v8::Value> value, AMX* amx);
        static cell To_Return_Code(v8::Isolate* isolate, v8::Local<v8::Context> context, v8::Local<v8::Value> js_value);
};
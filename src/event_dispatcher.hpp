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
#include <vector>
//
#include "samp-sdk/amx_defs.h"
//
#include "node_inclusion.hpp"

class Event_Dispatcher {
    public:
        static Event_Dispatcher& Instance();

        bool Has_Listeners(const std::string& event_name);
        std::string Get_Signature(const std::string& event_name);

        cell Dispatch_Event(const std::string& event_name, AMX* amx, const std::vector<v8::Local<v8::Value>>& args);

        cell Dispatch_Event(const std::string& event_name) {
            return Dispatch_Event(event_name, nullptr, {});
        }

        void Generate_Native_Bindings();
        void Generate_Hook_Bindings();
        void Generate_Public_Bindings();

    private:
        Event_Dispatcher() = default;
        ~Event_Dispatcher() = default;

        Event_Dispatcher(const Event_Dispatcher&) = delete;
        Event_Dispatcher& operator=(const Event_Dispatcher&) = delete;

        v8::Local<v8::Function> Get_Cached_Function(v8::Isolate* isolate, v8::Local<v8::Context> context, v8::Persistent<v8::Function>& cached_func, bool& is_cached, const char* global_name);
        v8::Local<v8::Function> Get_Emit_Function(v8::Isolate* isolate, v8::Local<v8::Context> context);
        v8::Local<v8::Function> Get_Has_Listeners_Function(v8::Isolate* isolate, v8::Local<v8::Context> context);
        v8::Local<v8::Function> Get_Signature_Function(v8::Isolate* isolate, v8::Local<v8::Context> context);

        v8::Persistent<v8::Function> cached_emit_func_;
        v8::Persistent<v8::Function> cached_has_listeners_func_;
        v8::Persistent<v8::Function> cached_signature_func_;

        bool emit_func_cached_ = false;
        bool has_listeners_func_cached_ = false;
        bool signature_func_cached_ = false;
};
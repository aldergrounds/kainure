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

#include <memory>
//
#include "node_inclusion.hpp"

class Runtime_Manager {
    public:
        static Runtime_Manager& Instance();

        bool Initialize();
        void Shutdown();
        void Process_Tick();

        bool Execute_Bootstrap_Script();

        v8::Isolate* Get_Isolate() const {
            return isolate_;
        }

        v8::Local<v8::Context> Get_Context() const;

        node::Environment* Get_Node_Environment() const {
            return node_env_;
        }

        uv_loop_t* Get_UV_Loop() const {
            return uv_loop_;
        }

        bool Is_Initialized() const {
            return is_initialized_;
        }

        bool Is_Runtime_Ready() const;

    private:
        Runtime_Manager() = default;
        ~Runtime_Manager() = default;

        Runtime_Manager(const Runtime_Manager&) = delete;
        Runtime_Manager& operator=(const Runtime_Manager&) = delete;

        bool Initialize_Platform();
        bool Initialize_Isolate();
        bool Initialize_Node_Environment();
        bool Load_Node_Environment();

        void Cleanup_Node_Environment();
        void Cleanup_Isolate();
        void Cleanup_Platform();

        void Setup_Console_Redirect();
        static void JS_Print(const v8::FunctionCallbackInfo<v8::Value>& info);

        bool Compile_TypeScript_If_Needed();

        std::unique_ptr<node::MultiIsolatePlatform> platform_;
        std::unique_ptr<node::ArrayBufferAllocator> allocator_;

        v8::Isolate* isolate_ = nullptr;
        node::IsolateData* isolate_data_ = nullptr;
        node::Environment* node_env_ = nullptr;
        uv_loop_t* uv_loop_ = nullptr;

        v8::Global<v8::Context> context_;

        bool is_initialized_ = false;
};
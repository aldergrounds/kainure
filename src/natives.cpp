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

#include <atomic>
//
#include "samp-sdk/amx_defs.h"
#include "samp-sdk/interceptor_manager.hpp"
#include "samp-sdk/callbacks.hpp"
#include "samp-sdk/platform.hpp"
//
#include "natives.hpp"
#include "native_hooks.hpp"
#include "type_converter.hpp"
#include "logger.hpp"
#include "exceptions.hpp"
#include "constants.hpp"

std::vector<std::unique_ptr<Native_Binding_Data>> Natives::bindings_storage_;

namespace {
    std::atomic<bool> has_any_hooks { false };

    struct Lazy_Sandbox {
        Samp_SDK::Detail::Amx_Sandbox sandbox;
        bool initialized = false;

        Lazy_Sandbox() : sandbox(Constants::AMX_SANDBOX_SIZE) {}

        SAMP_SDK_FORCE_INLINE AMX* Get() {
            if (!initialized) {
                sandbox.Reset();
                initialized = true;
            }
            else {
                sandbox.amx.stk = sandbox.amx.stp;
                sandbox.amx.hea = 0;
                sandbox.amx.paramcount = 0;
            }

            return &sandbox.amx;
        }
    };

    static thread_local Lazy_Sandbox tl_sandbox;
}

void Natives::Generate_Binding(v8::Isolate* isolate, v8::Local<v8::Object> target, const std::string& name, uint32_t hash) {
    try {
        if (!isolate)
            throw V8_Exception("Isolate is null in 'Generate_Binding'.");

        if (target.IsEmpty())
            throw V8_Exception("Target object is empty in 'Generate_Binding'.");

        v8::Local<v8::Context> context = isolate->GetCurrentContext();

        if (context.IsEmpty())
            throw V8_Exception("Context is empty in 'Generate_Binding'.");

        AMX_NATIVE native_func = Samp_SDK::Detail::Interceptor_Manager::Instance().Find_Cached_Native(hash);

        if (!native_func)
            return (void)Logger::Log(Log_Level::WARNING, "Native '%s' not found in cache during binding.", name.c_str());

        auto data = std::make_unique<Native_Binding_Data>();

        data->native_func = native_func;
        data->native_hash = hash;
        data->native_name = name;

        v8::Local<v8::External> external_data = v8::External::New(isolate, data.get());

        bindings_storage_.push_back(std::move(data));

        v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, Handler, external_data);
        v8::Local<v8::Function> fn;

        if (!tpl->GetFunction(context).ToLocal(&fn))
            throw V8_Exception("Failed to create function template for native '" + name + "'");

        v8::Local<v8::String> fn_name = v8::String::NewFromUtf8(isolate, name.c_str(), v8::NewStringType::kInternalized).ToLocalChecked();

        fn->SetName(fn_name);

        if (!target->Set(context, fn_name, fn).FromMaybe(false))
            Logger::Log(Log_Level::WARNING, "Failed to set native function '%s' on target object.", name.c_str());
    }
    catch (const V8_Exception& e) {
        Logger::Log(Log_Level::WARNING, "'%s'.", e.what());
    }
    catch (const std::exception& e) {
        Logger::Log(Log_Level::WARNING, "Unexpected error in 'Generate_Binding' for '%s': '%s'.", name.c_str(), e.what());
    }
}

void Natives::Clear_Bindings() {
    bindings_storage_.clear();
}

void Natives::Set_Has_Hooks(bool has_hooks) {
    has_any_hooks.store(has_hooks, std::memory_order_relaxed);
}

void Natives::Handler(const v8::FunctionCallbackInfo<v8::Value>& info) {
    v8::Isolate* isolate = info.GetIsolate();

    try {
        if (!isolate)
            throw V8_Exception("Native handler called with null isolate.");

        v8::HandleScope handle_scope(isolate);
        v8::Local<v8::Context> context = isolate->GetCurrentContext();

        if (context.IsEmpty())
            throw V8_Exception("Native handler called with empty context.");

        if (!info.Data()->IsExternal())
            throw V8_Exception("Invalid data in native handler.");

        Native_Binding_Data* data = static_cast<Native_Binding_Data*>(info.Data().As<v8::External>()->Value());

        if (!data || !data->native_func)
            throw AMX_Exception("Native function pointer is null.");

        if (has_any_hooks.load(std::memory_order_relaxed)) {
            if (!Native_Hooks::Instance().Dispatch(isolate, data->native_name, info))
                return;
        }

        int argc = info.Length();

        if (argc == 0) {
            AMX* amx_fake = tl_sandbox.Get();
            cell params[1] = { 0 };
            cell retval = data->native_func(amx_fake, params);

            info.GetReturnValue().Set(v8::Integer::New(isolate, retval));

            return;
        }

        AMX* amx_fake = tl_sandbox.Get();
        
        if (argc <= static_cast<int>(Constants::STACK_ARGS_THRESHOLD)) {
            Type_Converter::Conversion_Result conversions[Constants::STACK_ARGS_THRESHOLD];
            cell params_stack[Constants::STACK_BUFFER_SIZE] {};
            
            params_stack[0] = argc * sizeof(cell);
            
            for (int i = 0; i < argc; i++) {
                conversions[i] = Type_Converter::To_Cell(isolate, context, info[i], amx_fake);
                params_stack[i + 1] = conversions[i].value;
            }
            
            cell retval = data->native_func(amx_fake, params_stack);
            
            for (int i = 0; i < argc; i++) {
                if (conversions[i].Has_Updater())
                    conversions[i].updater();
            }
            
            info.GetReturnValue().Set(v8::Integer::New(isolate, retval));

            return;
        }

        std::vector<Type_Converter::Conversion_Result> conversions;
        conversions.reserve(argc);

        std::vector<cell> params_vec;
        params_vec.reserve(argc + 1);
        params_vec.push_back(argc * sizeof(cell));

        for (int i = 0; i < argc; i++) {
            conversions.push_back(Type_Converter::To_Cell(isolate, context, info[i], amx_fake));
            params_vec.push_back(conversions.back().value);
        }

        cell retval = data->native_func(amx_fake, params_vec.data());

        for (auto& conversion : conversions) {
            if (conversion.Has_Updater())
                conversion.updater();
        }

        info.GetReturnValue().Set(v8::Integer::New(isolate, retval));
    }
    catch (const Plugin_Exception& e) {
        Logger::Log(Log_Level::ERROR_s, "'%s'.", e.what());

        if (isolate)
            info.GetReturnValue().Set(v8::Undefined(isolate));
    }
    catch (const std::exception& e) {
        Logger::Log(Log_Level::ERROR_s, "Unexpected exception in native handler: '%s'.", e.what());

        if (isolate)
            info.GetReturnValue().Set(v8::Undefined(isolate));
    }
}
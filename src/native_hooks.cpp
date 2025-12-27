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

#include "native_hooks.hpp"
#include "error_handler.hpp"
#include "natives.hpp"

Native_Hooks& Native_Hooks::Instance() {
    static Native_Hooks instance;

    return instance;
}

void Native_Hooks::Register_Hook(v8::Isolate* isolate, const std::string& native_name, v8::Local<v8::Function> callback) {
    if (!isolate || callback.IsEmpty())
        return;

    auto entry = std::make_shared<Hook_Entry>(isolate, callback);
    hooks_[native_name].push_back(entry);
    
    if (hooks_.size() == 1 && hooks_[native_name].size() == 1)
        Natives::Set_Has_Hooks(true);
}

bool Native_Hooks::Dispatch(v8::Isolate* isolate, const std::string& native_name, const v8::FunctionCallbackInfo<v8::Value>& info) {
    auto it = hooks_.find(native_name);
    
    if (it == hooks_.end() || it->second.empty())
        return true;

    const auto& callbacks = it->second;
    v8::Local<v8::Context> context = isolate->GetCurrentContext();
    v8::Local<v8::Object> global = context->Global();

    int argc = info.Length();
    std::vector<v8::Local<v8::Value>> args(argc);

    for (int i = 0; i < argc; i++)
        args[i] = info[i];

    for (auto rit = callbacks.rbegin(); rit != callbacks.rend(); ++rit) {
        v8::Local<v8::Function> hook_fn = (*rit)->callback.Get(isolate);
        
        v8::TryCatch try_catch(isolate);
        v8::MaybeLocal<v8::Value> result_maybe = hook_fn->Call(context, global, argc, args.data());

        if (try_catch.HasCaught()) {
            Error_Handler::Log_Exception(isolate, try_catch);

            continue;
        }

        v8::Local<v8::Value> result;

        if (result_maybe.ToLocal(&result)) {
            if ((result->IsBoolean() && !result->BooleanValue(isolate)) || (result->IsNumber() && result->Int32Value(context).FromMaybe(1) == 0))
                return (info.GetReturnValue().Set(result), false);
        }
    }

    return true;
}

void Native_Hooks::Clear() {
    for (auto& pair : hooks_) {
        for (auto& entry : pair.second)
            entry->callback.Reset();
    }

    hooks_.clear();
    
    Natives::Set_Has_Hooks(false);
}
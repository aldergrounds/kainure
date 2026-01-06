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

#include <unordered_set> 
//
#include "sdk/amx_defs.h"
#include "sdk/interceptor_manager.hpp"
#include "sdk/public_dispatcher.hpp"
//
#include "event_dispatcher.hpp"
#include "runtime_manager.hpp"
#include "type_converter.hpp"
#include "error_handler.hpp"
#include "natives.hpp"
#include "native_hooks.hpp"
#include "publics.hpp"
#include "constants.hpp"
#include "logger.hpp"

Event_Dispatcher& Event_Dispatcher::Instance() {
    static Event_Dispatcher instance;

    return instance;
}

bool Event_Dispatcher::Has_Listeners(const std::string& event_name) {
    auto& runtime = Runtime_Manager::Instance();

    if (!runtime.Is_Runtime_Ready())
        return (Logger::Log(Log_Level::WARNING, "'Has_Listeners' called but runtime not ready."), false);

    v8::Isolate* isolate = runtime.Get_Isolate();
    
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    
    v8::Local<v8::Context> context = runtime.Get_Context();

    if (context.IsEmpty())
        return false;
    
    v8::Context::Scope context_scope(context);

    v8::Local<v8::Function> has_listeners_func = Get_Has_Listeners_Function(isolate, context);

    if (has_listeners_func.IsEmpty())
        return false;

    v8::Local<v8::Object> global = context->Global();
    v8::Local<v8::Value> event_name_val = v8::String::NewFromUtf8(isolate, event_name.c_str()).ToLocalChecked();
    v8::MaybeLocal<v8::Value> result_maybe = has_listeners_func->Call(context, global, 1, &event_name_val);
    v8::Local<v8::Value> result_val;

    if (result_maybe.ToLocal(&result_val) && result_val->IsBoolean())
        return result_val->BooleanValue(isolate);

    return false;
}

std::string Event_Dispatcher::Get_Signature(const std::string& event_name) {
    auto& runtime = Runtime_Manager::Instance();

    if (!runtime.Is_Runtime_Ready())
        return "";

    v8::Isolate* isolate = runtime.Get_Isolate();
    
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    
    v8::Local<v8::Context> context = runtime.Get_Context();

    if (context.IsEmpty())
        return "";
    
    v8::Context::Scope context_scope(context);

    v8::Local<v8::Function> sig_func = Get_Signature_Function(isolate, context);
    
    if (sig_func.IsEmpty())
        return "";

    v8::Local<v8::Object> global = context->Global();
    v8::Local<v8::Value> arg = v8::String::NewFromUtf8(isolate, event_name.c_str()).ToLocalChecked();

    v8::TryCatch try_catch(isolate);
    v8::MaybeLocal<v8::Value> result = sig_func->Call(context, global, 1, &arg);

    if (try_catch.HasCaught() || result.IsEmpty())
        return "";

    v8::Local<v8::Value> val = result.ToLocalChecked();
    
    if (!val->IsString())
        return "";

    v8::String::Utf8Value utf8(isolate, val);
    
    return std::string(*utf8);
}

cell Event_Dispatcher::Dispatch_Event(const std::string& event_name, AMX* amx, const std::vector<v8::Local<v8::Value>>& args) {
    auto& runtime = Runtime_Manager::Instance();

    if (!runtime.Is_Runtime_Ready())
        return PLUGIN_PUBLIC_CONTINUE;

    v8::Isolate* isolate = runtime.Get_Isolate();
    
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    
    v8::Local<v8::Context> context = runtime.Get_Context();

    if (context.IsEmpty())
        return PLUGIN_PUBLIC_CONTINUE;
    
    v8::Context::Scope context_scope(context);
    v8::TryCatch try_catch(isolate);

    v8::Local<v8::Function> emit_func = Get_Emit_Function(isolate, context);

    if (emit_func.IsEmpty())
        return PLUGIN_PUBLIC_CONTINUE;

    std::vector<v8::Local<v8::Value>> call_args;
    call_args.reserve(args.size() + 1);
    call_args.push_back(v8::String::NewFromUtf8(isolate, event_name.c_str(), v8::NewStringType::kNormal).ToLocalChecked());
    call_args.insert(call_args.end(), args.begin(), args.end());

    v8::Local<v8::Object> global = context->Global();
    v8::MaybeLocal<v8::Value> result_maybe = emit_func->Call(context, global, call_args.size(), call_args.data());

    if (try_catch.HasCaught())
        return (Error_Handler::Log_Exception(isolate, try_catch), PLUGIN_PUBLIC_CONTINUE);

    v8::Local<v8::Value> result_val;

    if (!result_maybe.ToLocal(&result_val))
        return PLUGIN_PUBLIC_CONTINUE;

    return Type_Converter::To_Return_Code(isolate, context, result_val);
}

void Event_Dispatcher::Generate_Native_Bindings() {
    auto& runtime = Runtime_Manager::Instance();

    if (!runtime.Is_Runtime_Ready())
        return (void)Logger::Log(Log_Level::WARNING, "Cannot generate native bindings: runtime not ready.");

    v8::Isolate* isolate = runtime.Get_Isolate();
    
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    
    v8::Local<v8::Context> context = runtime.Get_Context();

    if (context.IsEmpty())
        return;
    
    v8::Context::Scope context_scope(context);
    v8::TryCatch try_catch(isolate);

    const auto& all_natives = Samp_SDK::Detail::Interceptor_Manager::Instance().Get_Native_Name_Cache();

    v8::Local<v8::Object> global_obj = context->Global();
    v8::Local<v8::Value> kainure_val;

    if (!global_obj->Get(context, v8::String::NewFromUtf8(isolate, Constants::JS_KAINURE_OBJECT).ToLocalChecked()).ToLocal(&kainure_val) || !kainure_val->IsObject())
        return (void)Logger::Log(Log_Level::WARNING, "Object '%s' not found in JavaScript global scope.", Constants::JS_KAINURE_OBJECT);

    v8::Local<v8::Object> kainure_obj = kainure_val.As<v8::Object>();
    v8::Local<v8::Value> native_val;

    if (!kainure_obj->Get(context, v8::String::NewFromUtf8(isolate, Constants::JS_NATIVE_OBJECT).ToLocalChecked()).ToLocal(&native_val) || !native_val->IsObject())
        return (void)Logger::Log(Log_Level::WARNING, "Object '%s.%s' not found in JavaScript.", Constants::JS_KAINURE_OBJECT, Constants::JS_NATIVE_OBJECT);

    v8::Local<v8::Object> native_obj = native_val.As<v8::Object>();

    static const std::unordered_set<std::string> float_return_natives = {
        "float", "floatabs", "floatsqroot", "floatadd", "floatsub",
        "floatmul", "floatdiv", "floatsin", "floatcos", "floattan",
        "floatasin", "floatacos", "floatatan", "floatatan2", "floatlog",
        "floatfract", "floatpower", "NetStats_PacketLossPercent"
    };

    for (const auto& pair : all_natives) {
        char return_type = 'i';
        
        if (float_return_natives.find(pair.second) != float_return_natives.end()) {
            return_type = 'f';
        }

        Natives::Generate_Binding(isolate, native_obj, pair.second, pair.first, return_type);
    }

    if (try_catch.HasCaught())
        Error_Handler::Log_Exception(isolate, try_catch);
}

void Event_Dispatcher::Generate_Hook_Bindings() {
    auto& runtime = Runtime_Manager::Instance();

    if (!runtime.Is_Runtime_Ready())
        return (void)Logger::Log(Log_Level::WARNING, "Cannot generate hook bindings: runtime not ready.");

    v8::Isolate* isolate = runtime.Get_Isolate();
    
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    
    v8::Local<v8::Context> context = runtime.Get_Context();

    if (context.IsEmpty())
        return;
    
    v8::Context::Scope context_scope(context);
    v8::TryCatch try_catch(isolate);

    auto Register_Native_Hook_JS = [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        v8::Isolate* isolate = info.GetIsolate();

        if (info.Length() < 2 || !info[0]->IsString() || !info[1]->IsFunction())
            return;

        v8::String::Utf8Value native_name_utf8(isolate, info[0]);
        std::string native_name = *native_name_utf8;
        v8::Local<v8::Function> callback = info[1].As<v8::Function>();

        Native_Hooks::Instance().Register_Hook(isolate, native_name, callback);
    };

    v8::Local<v8::Function> fn;

    if (v8::Function::New(context, Register_Native_Hook_JS).ToLocal(&fn))
        context->Global()->Set(context, v8::String::NewFromUtf8(isolate, Constants::JS_REGISTER_NATIVE_HOOK).ToLocalChecked(), fn).Check();

    if (try_catch.HasCaught())
        Error_Handler::Log_Exception(isolate, try_catch);
}

void Event_Dispatcher::Generate_Public_Bindings() {
    auto& runtime = Runtime_Manager::Instance();

    if (!runtime.Is_Runtime_Ready())
        return (void)Logger::Log(Log_Level::WARNING, "Cannot generate public bindings: runtime not ready.");

    v8::Isolate* isolate = runtime.Get_Isolate();
    
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    
    v8::Local<v8::Context> context = runtime.Get_Context();

    if (context.IsEmpty())
        return;
    
    v8::Context::Scope context_scope(context);
    v8::TryCatch try_catch(isolate);

    v8::Local<v8::Object> global = context->Global();
    v8::Local<v8::Value> kainure_val;

    if (!global->Get(context, v8::String::NewFromUtf8(isolate, Constants::JS_KAINURE_OBJECT).ToLocalChecked()).ToLocal(&kainure_val) || !kainure_val->IsObject())
        return (void)Logger::Log(Log_Level::WARNING, "Object '%s' not found in JavaScript global scope (Generate_Public_Bindings).", Constants::JS_KAINURE_OBJECT);

    v8::Local<v8::Object> kainure_obj = kainure_val.As<v8::Object>();

    v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, Publics::Call_Handler);
    v8::Local<v8::Function> fn;

    if (tpl->GetFunction(context).ToLocal(&fn)) {
        v8::Local<v8::String> fn_name = v8::String::NewFromUtf8(isolate, Constants::JS_CALL_PUBLIC_OBJECT, v8::NewStringType::kInternalized).ToLocalChecked();
        fn->SetName(fn_name);

        kainure_obj->Set(context, fn_name, fn).Check();
    }

    if (try_catch.HasCaught())
        Error_Handler::Log_Exception(isolate, try_catch);
}

v8::Local<v8::Function> Event_Dispatcher::Get_Cached_Function(v8::Isolate* isolate, v8::Local<v8::Context> context, v8::Persistent<v8::Function>& cached_func, bool& is_cached, const char* global_name) {
    if (!isolate || context.IsEmpty())
        return v8::Local<v8::Function>();

    if (is_cached && !cached_func.IsEmpty())
        return cached_func.Get(isolate);

    v8::Local<v8::Object> global = context->Global();
    v8::Local<v8::Value> handler_val;

    if (!global->Get(context, v8::String::NewFromUtf8(isolate, global_name).ToLocalChecked()).ToLocal(&handler_val) || !handler_val->IsFunction())
        return v8::Local<v8::Function>();

    cached_func.Reset(isolate, handler_val.As<v8::Function>());
    is_cached = true;

    return cached_func.Get(isolate);
}

v8::Local<v8::Function> Event_Dispatcher::Get_Emit_Function(v8::Isolate* isolate, v8::Local<v8::Context> context) {
    return Get_Cached_Function(isolate, context, cached_emit_func_, emit_func_cached_, Constants::JS_EMIT_EVENT);
}

v8::Local<v8::Function> Event_Dispatcher::Get_Has_Listeners_Function(v8::Isolate* isolate, v8::Local<v8::Context> context) {
    return Get_Cached_Function(isolate, context, cached_has_listeners_func_, has_listeners_func_cached_, Constants::JS_HAS_LISTENERS);
}

v8::Local<v8::Function> Event_Dispatcher::Get_Signature_Function(v8::Isolate* isolate, v8::Local<v8::Context> context) {
    return Get_Cached_Function(isolate, context, cached_signature_func_, signature_func_cached_, Constants::JS_GET_SIGNATURE);
}
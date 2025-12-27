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

#include <vector>
//
#include "samp-sdk/amx_api.hpp"
#include "samp-sdk/amx_defs.h"
#include "samp-sdk/amx_manager.hpp"
#include "samp-sdk/interceptor_manager.hpp"
#include "samp-sdk/public_dispatcher.hpp"
//
#include "publics.hpp"
#include "event_dispatcher.hpp"
#include "runtime_manager.hpp"
#include "constants.hpp"
#include "logger.hpp"
#include "type_converter.hpp"
#include "exceptions.hpp"

bool Publics::Handler(const std::string& name, AMX* amx, cell& result) {
    try {
        if (name == Constants::EVENT_GAMEMODE_INIT)
            return true;

        auto& runtime = Runtime_Manager::Instance();

        if (!runtime.Is_Initialized())
            return true;

        if (!Event_Dispatcher::Instance().Has_Listeners(name))
            return true;

        v8::Isolate* isolate = runtime.Get_Isolate();

        if (!isolate)
            throw Runtime_Exception("Isolate is null in 'Publics::Handler'.");

        v8::Locker locker(isolate);
        v8::Isolate::Scope isolate_scope(isolate);
        v8::HandleScope handle_scope(isolate);
        v8::Local<v8::Context> context = runtime.Get_Context();

        if (context.IsEmpty())
            throw Runtime_Exception("Context is empty in 'Publics::Handler'.");

        v8::Context::Scope context_scope(context);

        if (!amx)
            throw AMX_Exception("AMX pointer is null in 'Publics::Handler'.");

        std::string signature = Event_Dispatcher::Instance().Get_Signature(name);
        size_t sig_len = signature.length();

        int param_count = amx->paramcount;
        std::vector<v8::Local<v8::Value>> js_args;
        js_args.reserve(param_count);

        for (int i = 0; i < param_count; i++) {
            cell param_value;

            if (!Samp_SDK::Detail::Public_Param_Reader::Get_Stack_Cell(amx, i, param_value)) {
                js_args.push_back(v8::Undefined(isolate));

                continue;
            }

            char type_char = (i < static_cast<int>(sig_len)) ? signature[i] : Constants::SIGNATURE_TYPE_INTEGER;

            switch (type_char) {
                case Constants::SIGNATURE_TYPE_FLOAT: {
                    float f_val = Samp_SDK::amx::AMX_CTOF(param_value);
                    js_args.push_back(v8::Number::New(isolate, f_val));

                    break;
                }
                case Constants::SIGNATURE_TYPE_STRING: {
                    cell* phys_addr = nullptr;

                    if (Samp_SDK::amx::Get_Addr(amx, param_value, &phys_addr) == static_cast<int>(Amx_Error::None) && phys_addr != nullptr) {
                        int len = 0;

                        if (Samp_SDK::amx::STR_Len(phys_addr, &len) == static_cast<int>(Amx_Error::None) && len >= 0) {
                            std::string str_val(len, '\0');
                            Samp_SDK::amx::Get_String(&str_val[0], phys_addr, len + 1);

                            if (!str_val.empty() && str_val.back() == '\0')
                                str_val.pop_back();

                            js_args.push_back(v8::String::NewFromUtf8(isolate, str_val.c_str(), v8::NewStringType::kNormal).ToLocalChecked());
                        }
                        else
                            js_args.push_back(v8::String::Empty(isolate));
                    }
                    else
                        js_args.push_back(v8::String::Empty(isolate));

                    break;
                }
                default:
                    js_args.push_back(v8::Integer::New(isolate, param_value));

                    break;
            }
        }

        cell return_value = Event_Dispatcher::Instance().Dispatch_Event(name, amx, js_args);
        result = return_value;

        if (name == Constants::EVENT_PLAYER_COMMAND) {
            if (return_value == PLUGIN_PUBLIC_CONTINUE)
                return false;
        }
        else {
            if (return_value == PLUGIN_PUBLIC_STOP)
                return false;
        }

        return true;
    }
    catch (const Plugin_Exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "Exception in 'Publics::Handler' for '%s': '%s'.", name.c_str(), e.what()), true);
    }
    catch (const std::exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "Unexpected exception in 'Publics::Handler' for '%s': '%s'.", name.c_str(), e.what()), true);
    }
}

bool Publics::Has_Handler(const std::string& name) {
    try {
        return Event_Dispatcher::Instance().Has_Listeners(name);
    }
    catch (const std::exception& e) {
        return (Logger::Log(Log_Level::WARNING, "Exception in 'Publics::Has_Handler': '%s'.", e.what()), false);
    }
}

void Publics::Call_Handler(const v8::FunctionCallbackInfo<v8::Value>& info) {
    v8::Isolate* isolate = info.GetIsolate();

    try {
        if (!isolate)
            throw V8_Exception("Isolate is null in 'Call_Handler'.");

        v8::HandleScope handle_scope(isolate);
        v8::Local<v8::Context> context = isolate->GetCurrentContext();

        if (info.Length() < 1 || !info[0]->IsString())
            throw AMX_Exception("Call_Public: First argument must be the function name.");

        v8::String::Utf8Value str_name(isolate, info[0]);
        if (!*str_name)
            throw AMX_Exception("Call_Public: Failed to convert function name to string.");

        std::string public_name = *str_name;

        int public_index = -1;
        AMX* target_amx = Samp_SDK::Amx_Manager::Instance().Find_Public(public_name.c_str(), public_index);

        if (!target_amx) {
            target_amx = Samp_SDK::Amx_Manager::Instance().Get_Primary_Amx();

            if (target_amx) {
                int find_result = Samp_SDK::amx::Find_Public(target_amx, public_name.c_str(), &public_index);

                if (find_result != 0)
                    target_amx = nullptr;
            }
        }

        if (!target_amx) {
            std::string err_msg = "The public '" + public_name + "' was not found.";
            isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, err_msg.c_str(), v8::NewStringType::kNormal).ToLocalChecked()));

            return;
        }

        int args_count = info.Length() - 1;
        
        std::vector<Type_Converter::Conversion_Result> converted_args;
        converted_args.reserve(args_count);

        for (int i = 0; i < args_count; i++)
            converted_args.push_back(Type_Converter::To_Cell(isolate, context, info[i + 1], target_amx));

        cell hea_before = target_amx->hea;
        cell stk_before = target_amx->stk;

        for (int i = args_count - 1; i >= 0; i--) {
            int push_result = Samp_SDK::amx::Push(target_amx, converted_args[i].value);

            if (push_result != static_cast<int>(Amx_Error::None))
                throw AMX_Exception("Failed to push argument " + std::to_string(i) + " to AMX stack.");
        }

        cell retval = 0;
        int exec_error = Samp_SDK::amx::Exec(target_amx, &retval, public_index);

        target_amx->hea = hea_before;
        target_amx->stk = stk_before;

        for (auto& conversion : converted_args) {
            if (conversion.Has_Updater())
                conversion.updater();
        }

        if (exec_error != static_cast<int>(Amx_Error::None) && exec_error != static_cast<int>(Amx_Error::Sleep)) {
            bool is_expected_ghost_error = (public_index == PLUGIN_EXEC_GHOST_PUBLIC && exec_error == static_cast<int>(Amx_Error::Index));

            if (!is_expected_ghost_error)
                Logger::Log(Log_Level::ERROR_s, "Error executing public '%s': Code '%d'.", public_name.c_str(), exec_error);
        }

        info.GetReturnValue().Set(v8::Integer::New(isolate, retval));
    }
    catch (const Plugin_Exception& e) {
        Logger::Log(Log_Level::ERROR_s, "'%s'.", e.what());

        if (isolate)
            isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, e.what()).ToLocalChecked()));
    }
    catch (const std::exception& e) {
        Logger::Log(Log_Level::ERROR_s, "Unexpected exception in 'Call_Handler': '%s'.", e.what());

        if (isolate)
            isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, "Internal error in 'Call_Public'").ToLocalChecked()));
    }
}
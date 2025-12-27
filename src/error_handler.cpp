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

#include <ctime>
#include <fstream>
#include <ios>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip>
//
#include "error_handler.hpp"
#include "constants.hpp"
#include "logger.hpp"

namespace {
    std::string Get_Current_Timestamp() {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm tm_buf;

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
        localtime_s(&tm_buf, &in_time_t);
#elif defined(LINUX) || defined(__linux__)
        localtime_r(&in_time_t, &tm_buf);
#endif

        std::stringstream ss;
        ss << std::put_time(&tm_buf, "%Y-%m-%d %X");

        return ss.str();
    }

    std::string Format_Exception_Details(v8::Isolate* isolate, v8::Local<v8::Context> context, v8::Local<v8::Value> exception) {
        if (!isolate || context.IsEmpty() || exception.IsEmpty())
            return "  Message: <invalid exception data>\n";

        std::stringstream ss;
        v8::String::Utf8Value exception_str(isolate, exception);

        const char* exception_cstr = *exception_str ? *exception_str : "<unknown error>";

        ss << "  Message: " << exception_cstr << "\n";

        if (exception->IsObject()) {
            v8::Local<v8::Object> exception_obj = exception.As<v8::Object>();
            v8::Local<v8::Value> stack_val;

            if (exception_obj->Get(context, v8::String::NewFromUtf8(isolate, "stack").ToLocalChecked()).ToLocal(&stack_val) && stack_val->IsString()) {
                v8::String::Utf8Value stack_trace(isolate, stack_val);

                if (*stack_trace)
                    ss << "  Stack Trace:\n" << *stack_trace;
            }
        }

        return ss.str();
    }

    bool Write_To_Log_File(const std::string& content) {
        if (content.empty())
            return true;

        std::ofstream log_file(Constants::FILE_ERROR_LOG, std::ios_base::app);

        if (!log_file.is_open())
            return false;

        log_file << content;

        return log_file.good();
    }

    void Write_Formatted_Log(const std::string& title, const std::string& content) {
        std::stringstream log_content;
        log_content << "================================================================\n";
        log_content << "[" << Get_Current_Timestamp() << "] " << title << ":\n";
        log_content << "----------------------------------------------------------------\n";
        log_content << content << "\n";
        log_content << "================================================================\n\n";

        if (!Write_To_Log_File(log_content.str()))
            Logger::Log(Log_Level::ERROR_s, "Failed to write to log file: '%s'.", Constants::FILE_ERROR_LOG);
    }
}

void Error_Handler::Log_Exception(v8::Isolate* isolate, v8::TryCatch& try_catch) {
    if (!isolate)
        return (void)Logger::Log(Log_Level::ERROR_s, "Cannot log exception: isolate is null.");

    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::Context> context = isolate->GetCurrentContext();

    if (context.IsEmpty())
        return (void)Logger::Log(Log_Level::ERROR_s, "Cannot log exception: context is empty.");

    v8::String::Utf8Value exception(isolate, try_catch.Exception());
    const char* exception_string = *exception ? *exception : "<unknown error>";

    std::stringstream ss;
    v8::Local<v8::Message> message = try_catch.Message();

    if (message.IsEmpty())
        ss << Format_Exception_Details(isolate, context, try_catch.Exception());
    else {
        v8::String::Utf8Value filename(isolate, message->GetScriptOrigin().ResourceName());
        int linenum = message->GetLineNumber(context).FromMaybe(-1);

        ss << "  File: " << (*filename ? *filename : "<unknown>") << " (Line: " << linenum << ")\n";
        ss << "  Message: " << exception_string << "\n";

        v8::Local<v8::Value> stack_trace_string;

        if (try_catch.StackTrace(context).ToLocal(&stack_trace_string) &&
            stack_trace_string->IsString() &&
            v8::Local<v8::String>::Cast(stack_trace_string)->Length() > 0) {

            v8::String::Utf8Value stack_trace(isolate, stack_trace_string);
            ss << "  Stack Trace:\n" << *stack_trace;
        }
    }

    Write_Formatted_Log("Unhandled JavaScript Exception", ss.str());

    Logger::Log(Log_Level::ERROR_s, "Unhandled JavaScript exception. Check '%s' for details.", Constants::FILE_ERROR_LOG);
    Logger::Log(Log_Level::ERROR_s, "   > %s", exception_string);
}

void Error_Handler::Promise_Rejection_Handler(v8::PromiseRejectMessage message) {
    if (message.GetEvent() != v8::kPromiseRejectWithNoHandler)
        return;

    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    if (!isolate)
        return (void)Logger::Log(Log_Level::ERROR_s, "Promise rejection handler called with null isolate.");

    v8::Locker locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);
    v8::Local<v8::Context> context = isolate->GetCurrentContext();

    if (context.IsEmpty())
        return (void)Logger::Log(Log_Level::ERROR_s, "Promise rejection handler called with empty context.");

    v8::Context::Scope context_scope(context);
    v8::Local<v8::Value> reason = message.GetValue();

    v8::String::Utf8Value reason_str(isolate, reason);
    const char* reason_cstr = *reason_str ? *reason_str : "<empty rejection reason>";

    Write_Formatted_Log("Unhandled Promise Rejection", Format_Exception_Details(isolate, context, reason));

    Logger::Log(Log_Level::ERROR_s, "Unhandled Promise rejection. Check '%s' for details.", Constants::FILE_ERROR_LOG);
    Logger::Log(Log_Level::ERROR_s, "   > %s", reason_cstr);
}
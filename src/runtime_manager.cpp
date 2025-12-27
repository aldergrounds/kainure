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
#include <string>
#include <sstream>
#include <algorithm>
#include <new>
//
#include "samp-sdk/logger.hpp"
//
#include "runtime_manager.hpp"
#include "file_manager.hpp"
#include "typescript_compiler.hpp"
#include "constants.hpp"
#include "error_handler.hpp"
#include "logger.hpp"
#include "exceptions.hpp"
#include "code_preprocessor.hpp"

Runtime_Manager & Runtime_Manager::Instance() {
    static Runtime_Manager instance;

    return instance;
}

bool Runtime_Manager::Is_Runtime_Ready() const {
    return is_initialized_ && isolate_ && !context_.IsEmpty();
}

bool Runtime_Manager::Initialize() {
    if (is_initialized_)
        return (Logger::Log(Log_Level::WARNING, "Runtime Manager already initialized."), true);

    try {
        Logger::Log(Log_Level::INFO, "Starting Runtime initialization...");

        if (!Initialize_Platform())
            throw Runtime_Exception("Failed to initialize V8 platform.");

        if (!Initialize_Isolate()) {
            Cleanup_Platform();

            throw Runtime_Exception("Failed to create V8 isolate.");
        }

        if (!Initialize_Node_Environment()) {
            Cleanup_Isolate();
            Cleanup_Platform();

            throw Runtime_Exception("Failed to create Node.js environment.");
        }

        if (!Load_Node_Environment()) {
            Cleanup_Node_Environment();
            Cleanup_Isolate();
            Cleanup_Platform();

            throw Runtime_Exception("Failed to load Node.js environment.");
        }

        Setup_Console_Redirect();

        is_initialized_ = true;

        Logger::Log(Log_Level::INFO, "Runtime initialized successfully.");

        return true;
    }
    catch (const Runtime_Exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "'%s'.", e.what()), false);
    }
    catch (const std::exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "Unexpected error during runtime initialization: '%s'.", e.what()), false);
    }
}

void Runtime_Manager::Shutdown() {
    if (!is_initialized_)
        return;

    try {
        Logger::Log(Log_Level::INFO, "Shutting down Runtime...");

        Cleanup_Node_Environment();
        Cleanup_Isolate();
        Cleanup_Platform();

        is_initialized_ = false;

        Logger::Log(Log_Level::INFO, "Runtime shut down successfully.");
    }
    catch (const std::exception& e) {
        Logger::Log(Log_Level::ERROR_s, "Error during runtime shutdown: '%s'.", e.what());
    }
}

void Runtime_Manager::Process_Tick() {
    if (!Is_Runtime_Ready() || !node_env_)
        return;

    try {
        v8::Locker locker(isolate_);
        v8::Isolate::Scope isolate_scope(isolate_);
        v8::HandleScope handle_scope(isolate_);
        v8::Local<v8::Context> context = context_.Get(isolate_);
        v8::Context::Scope context_scope(context);

        if (uv_loop_)
            uv_run(uv_loop_, UV_RUN_NOWAIT);

        if (platform_)
            platform_->DrainTasks(isolate_);
    }
    catch (const std::exception& e) {
        Logger::Log(Log_Level::ERROR_s, "Error in process tick: '%s'.", e.what());
    }
}

bool Runtime_Manager::Compile_TypeScript_If_Needed() {
    const auto& config = File_Manager::Instance().Get_Config();

    if (!config.use_typescript)
        return true;

    if (!TypeScript_Compiler::Instance().Is_TypeScript_File(config.main_file))
        return (Logger::Log(Log_Level::WARNING, "'use_typescript' is enabled but 'main_file' is not a '.ts' file."), true);

    if (!Is_Runtime_Ready())
        return (Logger::Log(Log_Level::ERROR_s, "Cannot compile TypeScript: runtime not ready."), false);

    v8::Locker locker(isolate_);
    v8::Isolate::Scope isolate_scope(isolate_);
    v8::HandleScope handle_scope(isolate_);
    v8::Local<v8::Context> context = context_.Get(isolate_);
    v8::Context::Scope context_scope(context);

    return TypeScript_Compiler::Instance().Compile_Project(isolate_, context);
}

bool Runtime_Manager::Execute_Bootstrap_Script() {
    if (!Is_Runtime_Ready())
        return (Logger::Log(Log_Level::ERROR_s, "Cannot execute bootstrap: runtime not ready."), false);

    if (!Compile_TypeScript_If_Needed())
        return (Logger::Log(Log_Level::ERROR_s, "TypeScript compilation failed."), false);

    v8::Locker locker(isolate_);
    v8::Isolate::Scope isolate_scope(isolate_);
    v8::HandleScope handle_scope(isolate_);
    v8::Local<v8::Context> context = context_.Get(isolate_);
    v8::Context::Scope context_scope(context);
    v8::TryCatch try_catch(isolate_);

    const auto& config = File_Manager::Instance().Get_Config();
    std::string main_file = config.main_file;

    if (config.use_typescript && TypeScript_Compiler::Instance().Is_TypeScript_File(main_file)) {
        main_file = TypeScript_Compiler::Instance().Get_Output_Path(main_file);
        Logger::Log(Log_Level::INFO, "Loading compiled JavaScript from: '%s'.", main_file.c_str());
    }

    std::replace(main_file.begin(), main_file.end(), '\\', '/');

    std::string bootstrap_code = "require('" + std::string(Constants::FILE_FRAMEWORK) + "');\n" + "require('" + main_file + "');";

    v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate_, bootstrap_code.c_str()).ToLocalChecked();
    v8::ScriptOrigin origin(isolate_, v8::String::NewFromUtf8(isolate_, Constants::FILE_BOOTSTRAP).ToLocalChecked());
    v8::MaybeLocal<v8::Script> script = v8::Script::Compile(context, source, &origin);

    if (script.IsEmpty()) {
        Logger::Log(Log_Level::ERROR_s, "Failed to compile bootstrap script.");
        Error_Handler::Log_Exception(isolate_, try_catch);

        return false;
    }

    v8::MaybeLocal<v8::Value> result = script.ToLocalChecked()->Run(context);

    if (result.IsEmpty())
        return (Error_Handler::Log_Exception(isolate_, try_catch), false);

    Logger::Log(Log_Level::INFO, "Bootstrap scripts loaded successfully.");

    return true;
}

v8::Local<v8::Context> Runtime_Manager::Get_Context() const {
    if (context_.IsEmpty())
        return v8::Local<v8::Context>();

    return context_.Get(isolate_);
}

void Runtime_Manager::Setup_Console_Redirect() {
    if (!isolate_ || context_.IsEmpty())
        return (void)Logger::Log(Log_Level::WARNING, "Cannot setup console redirect: isolate or context is empty.");

    v8::Locker locker(isolate_);
    v8::Isolate::Scope isolate_scope(isolate_);
    v8::HandleScope handle_scope(isolate_);
    v8::Local<v8::Context> context = context_.Get(isolate_);
    v8::Context::Scope context_scope(context);

    v8::Local<v8::Object> global = context->Global();
    v8::Local<v8::Value> console_val;

    if (!global->Get(context, v8::String::NewFromUtf8(isolate_, Constants::JS_CONSOLE_OBJECT).ToLocalChecked()).ToLocal(&console_val) || !console_val->IsObject())
        return;

    v8::Local<v8::Object> console_obj = console_val.As<v8::Object>();
    v8::Local<v8::Function> print_fn = v8::Function::New(context, JS_Print).ToLocalChecked();

    const char* console_methods[] = {
        Constants::CONSOLE_LOG,
        Constants::CONSOLE_INFO,
        Constants::CONSOLE_WARN,
        Constants::CONSOLE_ERROR,
        Constants::CONSOLE_DEBUG
    };

    for (const char* method : console_methods)
        console_obj->Set(context, v8::String::NewFromUtf8(isolate_, method).ToLocalChecked(), print_fn).Check();
}

void Runtime_Manager::JS_Print(const v8::FunctionCallbackInfo<v8::Value>& info) {
    v8::Isolate* isolate = info.GetIsolate();

    if (!isolate)
        return;

    std::stringstream ss;

    for (int i = 0; i < info.Length(); i++) {
        v8::String::Utf8Value str(isolate, info[i]);
        ss << (*str ? *str : "<invalid>");

        if (i < info.Length() - 1)
            ss << " ";
    }

    Samp_SDK::Log("%s", ss.str().c_str());
}

bool Runtime_Manager::Initialize_Platform() {
    Logger::Log(Log_Level::INFO, "Initializing Node.js process and V8 platform...");

    std::vector<std::string> args = { Constants::PROCESS_NAME };

    std::shared_ptr<node::InitializationResult> result = node::InitializeOncePerProcess(args, {
        node::ProcessInitializationFlags::kNoInitializeV8,
        node::ProcessInitializationFlags::kNoInitializeNodeV8Platform
    });

    if (result->early_return()) {
        Logger::Log(Log_Level::ERROR_s, "Node.js initialization failed with exit code: '%d'.", result->exit_code());

        for (const std::string& err : result->errors())
            Logger::Log(Log_Level::ERROR_s, "   > %s", err.c_str());

        return false;
    }

    Logger::Log(Log_Level::INFO, "Node.js process initialized.");

    platform_ = node::MultiIsolatePlatform::Create(Constants::V8_PLATFORM_THREADS);

    if (!platform_)
        return (Logger::Log(Log_Level::ERROR_s, "Failed to create V8 platform."), false);

    v8::V8::InitializePlatform(platform_.get());
    v8::V8::Initialize();

    uv_loop_ = new(std::nothrow) uv_loop_t();

    if (!uv_loop_)
        return (Logger::Log(Log_Level::ERROR_s, "Failed to allocate UV loop."), false);

    int uv_result = uv_loop_init(uv_loop_);

    if (uv_result != 0) {
        Logger::Log(Log_Level::ERROR_s, "Failed to initialize UV loop: '%s'.", uv_strerror(uv_result));

        delete uv_loop_;
        uv_loop_ = nullptr;

        return false;
    }

    allocator_ = node::ArrayBufferAllocator::Create();

    if (!allocator_)
        return (Logger::Log(Log_Level::ERROR_s, "Failed to create array buffer allocator."), false);

    Logger::Log(Log_Level::INFO, "V8 Platform and libuv initialized.");

    return true;
}

bool Runtime_Manager::Initialize_Isolate() {
    Logger::Log(Log_Level::INFO, "Creating V8 Isolate...");

    if (!allocator_ || !uv_loop_ || !platform_)
        return (Logger::Log(Log_Level::ERROR_s, "Cannot create isolate: dependencies not initialized."), false);

    isolate_ = node::NewIsolate(allocator_.get(), uv_loop_, platform_.get());

    if (!isolate_)
        return (Logger::Log(Log_Level::ERROR_s, "'node::NewIsolate' returned nullptr."), false);

    isolate_->SetPromiseRejectCallback(Error_Handler::Promise_Rejection_Handler);

    v8::Locker locker(isolate_);
    v8::Isolate::Scope isolate_scope(isolate_);
    v8::HandleScope handle_scope(isolate_);

    isolate_data_ = node::CreateIsolateData(isolate_, uv_loop_, platform_.get(), allocator_.get());

    if (!isolate_data_)
        return (Logger::Log(Log_Level::ERROR_s, "'node::CreateIsolateData' returned nullptr."), false);

    Logger::Log(Log_Level::INFO, "V8 Isolate created.");

    return true;
}

bool Runtime_Manager::Initialize_Node_Environment() {
    Logger::Log(Log_Level::INFO, "Creating Node.js Context and Environment...");

    if (!isolate_ || !isolate_data_)
        return (Logger::Log(Log_Level::ERROR_s, "Cannot create Node environment: isolate not initialized."), false);

    v8::Locker locker(isolate_);
    v8::Isolate::Scope isolate_scope(isolate_);
    v8::HandleScope handle_scope(isolate_);

    v8::Local<v8::Context> context = node::NewContext(isolate_);

    if (context.IsEmpty())
        return (Logger::Log(Log_Level::ERROR_s, "'node::NewContext' returned empty context."), false);

    context_.Reset(isolate_, context);
    v8::Context::Scope context_scope(context);

    auto Preprocess_Function = [](const v8::FunctionCallbackInfo<v8::Value>& info) {
        v8::Isolate* isolate = info.GetIsolate();

        if (info.Length() < 1 || !info[0]->IsString())
            return;

        v8::String::Utf8Value code_utf8(isolate, info[0]);
        std::string code = *code_utf8;

        std::string transformed = Code_Preprocessor::Instance().Transform_Native_Calls(code);

        info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, transformed.c_str()).ToLocalChecked());
    };

    context->Global()->Set(context, v8::String::NewFromUtf8(isolate_, Constants::JS_PREPROCESS_FUNCTION).ToLocalChecked(), v8::Function::New(context, Preprocess_Function).ToLocalChecked()).Check();

    std::vector<std::string> args = { Constants::PROCESS_NAME };
    std::vector<std::string> exec_args;

    node::EnvironmentFlags::Flags flags = static_cast<node::EnvironmentFlags::Flags>(0);
    node_env_ = node::CreateEnvironment(isolate_data_, context, args, exec_args, flags);

    if (!node_env_)
        return (Logger::Log(Log_Level::ERROR_s, "'node::CreateEnvironment' returned nullptr."), false);

    Logger::Log(Log_Level::INFO, "Node.js Context and Environment created.");

    return true;
}

bool Runtime_Manager::Load_Node_Environment() {
    Logger::Log(Log_Level::INFO, "Loading Node.js Environment (bootstrap)...");

    if (!isolate_ || !node_env_)
        return (Logger::Log(Log_Level::ERROR_s, "Cannot load Node environment: not initialized."), false);

    v8::Locker locker(isolate_);
    v8::Isolate::Scope isolate_scope(isolate_);
    v8::HandleScope handle_scope(isolate_);
    v8::Local<v8::Context> context = context_.Get(isolate_);

    if (context.IsEmpty())
        return (Logger::Log(Log_Level::ERROR_s, "Cannot load Node environment: context is empty."), false);

    v8::Context::Scope context_scope(context);
    v8::TryCatch try_catch(isolate_);

    std::string bootstrap_code = R"(
        delete process.stdin;
        const public_require = require('module').createRequire(process.cwd() + '/');
    
        const _module = require('module');
        const original_compile = _module.prototype._compile;
    
        _module.prototype._compile = function(content, filename) {
            if (filename.endsWith(')" + std::string(Constants::JS_EXTENSION) + R"('))
                content = globalThis.)" + std::string(Constants::JS_PREPROCESS_FUNCTION) + R"((content);
        
            return original_compile.call(this, content, filename);
        };
    
        globalThis.require = public_require;
    )";

    v8::MaybeLocal<v8::Value> loadenv_result = node::LoadEnvironment(node_env_, bootstrap_code.c_str());

    if (try_catch.HasCaught())
        return (Error_Handler::Log_Exception(isolate_, try_catch), false);

    if (loadenv_result.IsEmpty())
        return (Logger::Log(Log_Level::ERROR_s, "LoadEnvironment returned empty value."), false);

    Logger::Log(Log_Level::INFO, "Node.js Environment loaded.");

    return true;
}

void Runtime_Manager::Cleanup_Node_Environment() {
    if (!node_env_)
        return;

    try {
        if (isolate_) {
            v8::Locker locker(isolate_);
            v8::Isolate::Scope isolate_scope(isolate_);
            v8::HandleScope handle_scope(isolate_);
            v8::Local<v8::Context> context = context_.Get(isolate_);

            if (!context.IsEmpty()) {
                v8::Context::Scope context_scope(context);
                node::EmitProcessBeforeExit(node_env_);
                node::EmitProcessExit(node_env_);
            }
        }

        if (uv_loop_)
            uv_run(uv_loop_, UV_RUN_DEFAULT);

        node::FreeEnvironment(node_env_);
        node_env_ = nullptr;

        if (isolate_data_) {
            node::FreeIsolateData(isolate_data_);
            isolate_data_ = nullptr;
        }
    }
    catch (const std::exception& e) {
        Logger::Log(Log_Level::ERROR_s, "Error cleaning up Node environment: '%s'.", e.what());
    }
}

void Runtime_Manager::Cleanup_Isolate() {
    if (!isolate_)
        return;

    try {
        if (platform_)
            platform_->UnregisterIsolate(isolate_);

        isolate_->Dispose();
        isolate_ = nullptr;
    }
    catch (const std::exception& e) {
        Logger::Log(Log_Level::ERROR_s, "Error cleaning up isolate: '%s'.", e.what());
    }
}

void Runtime_Manager::Cleanup_Platform() {
    try {
        node::TearDownOncePerProcess();

        if (uv_loop_) {
            for (int i = 0; i < Constants::UV_LOOP_CLOSE_RETRY_LIMIT; i++) {
                int result = uv_loop_close(uv_loop_);

                if (result == 0)
                    break;

                if (i < Constants::UV_LOOP_CLOSE_RETRY_LIMIT - 1)
                    uv_run(uv_loop_, UV_RUN_NOWAIT);
            }

            delete uv_loop_;
            uv_loop_ = nullptr;
        }

        platform_.reset();
        allocator_.reset();
    }
    catch (const std::exception& e) {
        Logger::Log(Log_Level::ERROR_s, "Error cleaning up platform: '%s'.", e.what());
    }
}
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

#include <filesystem>
#include <algorithm>
#include <cstdint>
//
#include "typescript_compiler.hpp"
#include "file_manager.hpp"
#include "constants.hpp"
#include "logger.hpp"
#include "error_handler.hpp"
#include "exceptions.hpp"

namespace fs = std::filesystem;

TypeScript_Compiler& TypeScript_Compiler::Instance() {
    static TypeScript_Compiler instance;

    return instance;
}

bool TypeScript_Compiler::Is_TypeScript_File(const std::string& path) const {
    size_t ext_len = strlen(Constants::TS_EXTENSION);
    return path.length() >= ext_len && path.substr(path.length() - ext_len) == Constants::TS_EXTENSION;
}

std::string TypeScript_Compiler::Get_Output_Path(const std::string& ts_path) const {
    try {
        const auto& config = File_Manager::Instance().Get_Config();
        std::string output_dir = config.ts_output_dir.empty() ? Constants::DEFAULT_TS_OUT_DIR : config.ts_output_dir;

        fs::path input_path(ts_path);
        fs::path out_path(output_dir);

        out_path /= input_path;
        out_path.replace_extension(Constants::JS_EXTENSION);

        std::string final_path = out_path.string();
        std::replace(final_path.begin(), final_path.end(), '\\', '/');

        return final_path;
    }
    catch (const fs::filesystem_error& e) {
        return (Logger::Log(Log_Level::ERROR_s, "Filesystem error in 'Get_Output_Path': '%s'.", e.what()), "");
    }
    catch (const std::exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "Error in 'Get_Output_Path': '%s'.", e.what()), "");
    }
}

bool TypeScript_Compiler::Check_TypeScript_Installation(v8::Isolate* isolate, v8::Local<v8::Context> context) {
    try {
        if (!isolate)
            throw V8_Exception("Isolate is null in 'Check_TypeScript_Installation'.");

        if (context.IsEmpty())
            throw V8_Exception("Context is empty in 'Check_TypeScript_Installation'.");

        Logger::Log(Log_Level::INFO, "Checking TypeScript installation...");

        const char* check_code = R"(
            (function() {
                try {
                    require.resolve('typescript');
                    
                    return true;
                }
                catch(e) {
                    return false;
                }
            })()
        )";

        v8::TryCatch try_catch(isolate);
        v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, check_code).ToLocalChecked();
        v8::ScriptOrigin origin(isolate, v8::String::NewFromUtf8(isolate, Constants::TS_CHECK_SCRIPT_NAME).ToLocalChecked());

        v8::MaybeLocal<v8::Script> script = v8::Script::Compile(context, source, &origin);

        if (script.IsEmpty()) {
            Error_Handler::Log_Exception(isolate, try_catch);

            throw Script_Exception("Failed to compile TypeScript check script.");
        }

        v8::MaybeLocal<v8::Value> result = script.ToLocalChecked()->Run(context);

        if (result.IsEmpty())
            throw Script_Exception("TypeScript check script returned empty result.");

        bool is_installed = result.ToLocalChecked()->BooleanValue(isolate);

        if (is_installed)
            Logger::Log(Log_Level::INFO, "TypeScript is installed.");

        return is_installed;
    }
    catch (const Plugin_Exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "'%s'.", e.what()), false);
    }
    catch (const std::exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "Unexpected error checking TypeScript: '%s'.", e.what()), false);
    }
}

bool TypeScript_Compiler::Execute_TypeScript_Compiler(v8::Isolate* isolate, v8::Local<v8::Context> context) {
    try {
        if (!isolate)
            throw V8_Exception("Isolate is null in 'Execute_TypeScript_Compiler'.");

        if (context.IsEmpty())
            throw V8_Exception("Context is empty in 'Execute_TypeScript_Compiler'.");

        Logger::Log(Log_Level::INFO, "Compiling TypeScript project...");

        const auto& config = File_Manager::Instance().Get_Config();
        std::string output_dir = config.ts_output_dir.empty() ? Constants::DEFAULT_TS_OUT_DIR : config.ts_output_dir;

        try {
            if (!fs::exists(output_dir)) {
                if (!fs::create_directories(output_dir))
                    throw File_Exception("Failed to create output directory: " + output_dir);
            }
        }
        catch (const fs::filesystem_error& e) {
            throw File_Exception(std::string("Filesystem error creating output directory: ") + e.what());
        }

        std::string safe_output_dir = output_dir;
        std::string safe_tsconfig = Constants::FILE_TSCONFIG;

        for (char& c : safe_output_dir) {
            if (c == '\\')
                c = '/';
        }

        for (char& c : safe_tsconfig) {
            if (c == '\\')
                c = '/';
        }

        std::string compile_code = R"(
            (function() {
                const ts = require('typescript');
                const fs = require('fs');
                const path = require('path');
        
                try {
                    const tsconfig_path = ')" + safe_tsconfig + R"(';
                    const forced_out_dir = ')" + safe_output_dir + R"(';

                    if (!fs.existsSync(tsconfig_path))
                        throw new Error('tsconfig.json not found at: ' + tsconfig_path);

                    const config_file = ts.readConfigFile(tsconfig_path, ts.sys.readFile);

                    if (config_file.error) {
                         const error = ts.formatDiagnostics([config_file.error], {
                            getCanonicalFileName: f => f,
                            getCurrentDirectory: ts.sys.getCurrentDirectory,
                            getNewLine: () => '\n'
                        });

                        throw new Error('Error reading tsconfig.json:\n' + error);
                    }

                    const parsed_config = ts.parseJsonConfigFileContent(
                        config_file.config,
                        ts.sys,
                        path.dirname(tsconfig_path)
                    );

                    if (parsed_config.errors.length > 0) {
                        const errors = ts.formatDiagnostics(parsed_config.errors, {
                            getCanonicalFileName: f => f,
                            getCurrentDirectory: ts.sys.getCurrentDirectory,
                            getNewLine: () => '\n'
                        });

                        throw new Error('Error parsing tsconfig.json content:\n' + errors);
                    }

                    parsed_config.options.outDir = forced_out_dir;
                    
                    if (!parsed_config.options.rootDir)
                        parsed_config.options.rootDir = './';

                    const program = ts.createProgram(parsed_config.fileNames, parsed_config.options);
                    const emit_result = program.emit();
                    const all_diagnostics = ts.getPreEmitDiagnostics(program).concat(emit_result.diagnostics);
            
                    if (all_diagnostics.length > 0) {
                        const errors = [];
                        const warnings = [];
                
                        all_diagnostics.forEach(diagnostic => {
                            const message = ts.flattenDiagnosticMessageText(diagnostic.messageText, '\n');
                            let formatted_message = '';
                    
                            if (diagnostic.file && diagnostic.start !== undefined) {
                                const { line, character } = diagnostic.file.getLineAndCharacterOfPosition(diagnostic.start);
                                formatted_message = `${diagnostic.file.fileName} (${line + 1},${character + 1}): ${message}`;
                            }
                            else
                                formatted_message = message;

                            if (diagnostic.category === ts.DiagnosticCategory.Error)
                                errors.push(formatted_message);
                            else if (diagnostic.category === ts.DiagnosticCategory.Warning)
                                warnings.push(formatted_message);
                        });
                
                        if (errors.length > 0)
                            throw new Error('TypeScript compilation failed with errors:\n' + errors.join('\n'));
                
                        if (warnings.length > 0) {
                            return {
                                success: true,
                                warnings: warnings
                            };
                        }
                    }
            
                    return {
                        success: true,
                        warnings: []
                    };
                }
                catch (error) {
                    throw new Error('TypeScript compilation error: ' + error.message);
                }
            })()
        )";

        v8::TryCatch try_catch(isolate);
        v8::Local<v8::String> source = v8::String::NewFromUtf8(isolate, compile_code.c_str()).ToLocalChecked();
        v8::ScriptOrigin origin(isolate, v8::String::NewFromUtf8(isolate, Constants::TS_COMPILER_SCRIPT_NAME).ToLocalChecked());
        v8::MaybeLocal<v8::Script> script = v8::Script::Compile(context, source, &origin);

        if (script.IsEmpty()) {
            Error_Handler::Log_Exception(isolate, try_catch);

            throw Script_Exception("Failed to compile TypeScript compiler script.");
        }

        v8::MaybeLocal<v8::Value> result = script.ToLocalChecked()->Run(context);

        if (result.IsEmpty()) {
            Error_Handler::Log_Exception(isolate, try_catch);

            throw TypeScript_Exception("TypeScript compilation failed.");
        }

        v8::Local<v8::Value> result_val = result.ToLocalChecked();

        if (result_val->IsObject()) {
            v8::Local<v8::Object> result_obj = result_val.As<v8::Object>();
            v8::Local<v8::Value> warnings_val;

            if (result_obj->Get(context, v8::String::NewFromUtf8(isolate, "warnings").ToLocalChecked()).ToLocal(&warnings_val) &&
                warnings_val->IsArray()) {

                v8::Local<v8::Array> warnings_arr = warnings_val.As<v8::Array>();
                uint32_t warnings_count = warnings_arr->Length();

                if (warnings_count > 0) {
                    Logger::Log(Log_Level::WARNING, "TypeScript compilation completed with '%u' warning(s):", warnings_count);

                    for (uint32_t i = 0; i < warnings_count; i++) {
                        v8::Local<v8::Value> warning_val;

                        if (warnings_arr->Get(context, i).ToLocal(&warning_val) && warning_val->IsString()) {
                            v8::String::Utf8Value warning_str(isolate, warning_val);
                            Logger::Log(Log_Level::WARNING, "   > %s", *warning_str);
                        }
                    }
                }
            }
        }

        Logger::Log(Log_Level::INFO, "TypeScript compilation completed successfully.");

        return true;
    }
    catch (const Plugin_Exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "'%s'.", e.what()), false);
    }
    catch (const std::exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "Unexpected error compiling TypeScript: '%s'.", e.what()), false);
    }
}

bool TypeScript_Compiler::Compile_Project(v8::Isolate* isolate, v8::Local<v8::Context> context) {
    try {
        bool is_installed = Check_TypeScript_Installation(isolate, context);

        if (!is_installed) {
            Logger::Log(Log_Level::ERROR_s, "TypeScript module not found!");
            Logger::Log(Log_Level::ERROR_s, "Please install TypeScript manually by running:");
            Logger::Log(Log_Level::ERROR_s, "- 'npm install typescript'");
            Logger::Log(Log_Level::ERROR_s, "The server cannot start without TypeScript when 'typescript.enabled' is enabled.");
            
            throw TypeScript_Exception("TypeScript module not found.");
        }

        return Execute_TypeScript_Compiler(isolate, context);
    }
    catch (const TypeScript_Exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "'%s'.", e.what()), false);
    }
    catch (const std::exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "Unexpected error in 'Compile_Project': '%s'.", e.what()), false);
    }
}
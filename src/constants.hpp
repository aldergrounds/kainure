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

namespace Constants {
    // Directories
    constexpr const char* DIR_ROOT = "Kainure";
    constexpr const char* DIR_CORE = "Kainure/core";
    constexpr const char* DIR_TYPES = "Kainure/types";
    constexpr const char* DIR_INCLUDES = "Kainure/includes";
    constexpr const char* DIR_INCLUDES_STORAGE = "Kainure/includes_storage";

    // Files
    constexpr const char* FILE_ERROR_LOG = "Kainure/errors.log";
    constexpr const char* FILE_CONFIG = "Kainure/config.json";
    constexpr const char* FILE_FRAMEWORK = "./Kainure/kainure.js";
    constexpr const char* FILE_BOOTSTRAP = "bootstrap.js";
    constexpr const char* FILE_TSCONFIG = "tsconfig.json";

    // Default paths
    constexpr const char* DEFAULT_MAIN_FILE = "./main.js";
    constexpr const char* DEFAULT_TS_OUT_DIR = "./dist";

    // Names of global JavaScript functions
    constexpr const char* JS_EMIT_EVENT = "Kainure_Emit_Event";
    constexpr const char* JS_HAS_LISTENERS = "Kainure_Has_Listeners";
    constexpr const char* JS_GET_SIGNATURE = "Kainure_Get_Signature";
    constexpr const char* JS_REGISTER_NATIVE_HOOK = "Kainure_Register_Native_Hook";
    constexpr const char* JS_KAINURE_OBJECT = "Kainure";
    constexpr const char* JS_NATIVE_OBJECT = "native";
    constexpr const char* JS_CALL_PUBLIC_OBJECT = "call_public";
    constexpr const char* JS_CONSOLE_OBJECT = "console";
    constexpr const char* JS_PREPROCESS_FUNCTION = "Kainure_Preprocess";

    // V8/Node.js settings
    constexpr const char* PROCESS_NAME = "Kainure.dll";
    constexpr int V8_PLATFORM_THREADS = 4;

    // Buffer sizes
    constexpr size_t DEFAULT_STRING_BUFFER_SIZE = 4096;
    constexpr size_t LOG_BUFFER_SIZE = 2048;
    constexpr size_t AMX_SANDBOX_SIZE = 64 * 1024;

    // Special events
    constexpr const char* EVENT_GAMEMODE_INIT = "OnGameModeInit";
    constexpr const char* EVENT_PLAYER_COMMAND = "OnPlayerCommandText";

    // Reference types (for JS wrapping)
    constexpr const char* VALUE_FIELD = "value";
    constexpr const char* PARENT_FIELD = "parent";

    constexpr const char* KAINURE_REF_FIELD = "__kainure_ref";
    constexpr const char* KAINURE_PTR_FIELD = "__kainure_ptr";

    // JSON config keys
    constexpr const char* CONFIG_KEY_CONFIGS = "configs";
    constexpr const char* CONFIG_KEY_MAIN_FILE = "main_file";

    constexpr const char* CONFIG_KEY_TYPESCRIPT = "typescript";
    constexpr const char* CONFIG_KEY_TS_ENABLED = "enabled";
    constexpr const char* CONFIG_KEY_TS_OUT_DIR = "output_dir";
    constexpr const char* CONFIG_KEY_TS_AUTO_INSTALL = "auto_install";

    // Console methods
    constexpr const char* CONSOLE_LOG = "log";
    constexpr const char* CONSOLE_INFO = "info";
    constexpr const char* CONSOLE_WARN = "warn";
    constexpr const char* CONSOLE_ERROR = "error";
    constexpr const char* CONSOLE_DEBUG = "debug";

    // UV loop modes
    constexpr int UV_LOOP_CLOSE_RETRY_LIMIT = 2;

    // TypeScript related constants
    constexpr const char* JS_EXTENSION = ".js";
    constexpr const char* TS_EXTENSION = ".ts";
    constexpr const char* TS_CHECK_SCRIPT_NAME = "ts_check.js";
    constexpr const char* TS_INSTALL_SCRIPT_NAME = "ts_install.js";
    constexpr const char* TS_COMPILER_SCRIPT_NAME = "ts_compiler.js";
    constexpr const char* NPM_INSTALL_TYPESCRIPT = "npm install typescript";
    constexpr int NPM_INSTALL_TIMEOUT_MS = 120000;

    // TypeScript Config defaults
    constexpr const char* TSCONFIG_TARGET = "ES2020";
    constexpr const char* TSCONFIG_MODULE = "commonjs";
    constexpr const char* TSCONFIG_MODULE_RESOLUTION = "node";
    constexpr const char* TSCONFIG_ROOT_DIR = "./";
    constexpr const char* TSCONFIG_INCLUDE_PATTERN = "**/*.ts";
    constexpr const char* TSCONFIG_EXCLUDE_NODE_MODULES = "node_modules";

    // Parameter signature types
    constexpr char SIGNATURE_TYPE_INTEGER = 'i';
    constexpr char SIGNATURE_TYPE_FLOAT = 'f';
    constexpr char SIGNATURE_TYPE_STRING = 's';

    // Native function call optimization
    constexpr size_t STACK_ARGS_THRESHOLD = 16;
    constexpr size_t STACK_BUFFER_SIZE = 17;
}
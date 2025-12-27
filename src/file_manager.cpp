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
#include <fstream>
//
#include "nlohmann/json.hpp"
//
#include "file_manager.hpp"
#include "constants.hpp"
#include "logger.hpp"
#include "exceptions.hpp"

namespace fs = std::filesystem;

namespace {
    template<typename T>
    bool Load_JSON_Field(const nlohmann::ordered_json& j, const char* key, T& out_value, const T& default_value, nlohmann::ordered_json& parent, bool& save_needed) {
        if (j.contains(key)) {
            try {
                return (out_value = j[key].get<T>(), true);
            }
            catch (const nlohmann::json::exception& e) {
                Logger::Log(Log_Level::WARNING, "'%s' has invalid type in config: %s. Using default.", key, e.what());
            }
        }

        Logger::Log(Log_Level::WARNING, "'%s' missing or invalid in config. Adding default value.", key);

        out_value = default_value;
        parent[key] = default_value;
        save_needed = true;

        return false;
    }

    bool Ensure_JSON_Section(nlohmann::ordered_json& j, const char* section_name, bool& save_needed) {
        if (!j.contains(section_name) || !j[section_name].is_object()) {
            Logger::Log(Log_Level::WARNING, "'%s' section missing in config. Adding default values.", section_name);

            j[section_name] = nlohmann::ordered_json::object();
            save_needed = true;

            return false;
        }

        return true;
    }

    bool Save_JSON_Config(const nlohmann::ordered_json& j, const char* filepath) {
        try {
            std::ofstream out_file(filepath);

            if (!out_file.is_open())
                throw File_Exception(std::string("Failed to open file for writing: ") + filepath);

            out_file << j.dump(4);
            
            if (!out_file.good())
                throw File_Exception(std::string("Failed to write to file: ") + filepath);

            Logger::Log(Log_Level::INFO, "Config file updated with missing keys.");

            return true;
        }
        catch (const File_Exception& e) {
            return (Logger::Log(Log_Level::ERROR_s, "'%s'.", e.what()), false);
        }
        catch (const std::exception& e) {
            return (Logger::Log(Log_Level::ERROR_s, "Unexpected error saving config: '%s'.", e.what()), false);
        }
    }
}

File_Manager& File_Manager::Instance() {
    static File_Manager instance;

    return instance;
}

const File_Manager::Config& File_Manager::Get_Config() const {
    return config_;
}

bool File_Manager::Initialize() {
    try {
        if (!Create_Directories())
            throw Initialization_Exception("Failed to create required directories.");

        if (!Load_Config())
            throw Initialization_Exception("Failed to load configuration.");

        return true;
    }
    catch (const Plugin_Exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "'File_Manager' initialization failed: '%s'.", e.what()), false);
    }
    catch (const std::exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "Unexpected error during 'File_Manager' initialization: '%s'.", e.what()), false);
    }
}

bool File_Manager::Create_Directories() {
    try {
        const char* directories[] = {
            Constants::DIR_ROOT,
            Constants::DIR_CORE,
            Constants::DIR_TYPES,
            Constants::DIR_INCLUDES,
            Constants::DIR_INCLUDES_STORAGE
        };

        for (const char* dir : directories) {
            if (!fs::exists(dir)) {
                if (!fs::create_directory(dir))
                    throw File_Exception(std::string("Failed to create directory: ") + dir);
            }
        }

        return true;
    }
    catch (const fs::filesystem_error& e) {
        return (Logger::Log(Log_Level::ERROR_s, "Filesystem error while creating directories: '%s'.", e.what()), false);
    }
    catch (const File_Exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "'%s'.", e.what()), false);
    }
}

void File_Manager::Create_Default_Config() {
    try {
        nlohmann::ordered_json j;
        
        nlohmann::ordered_json configs;
        configs[Constants::CONFIG_KEY_MAIN_FILE] = Constants::DEFAULT_MAIN_FILE;
        j[Constants::CONFIG_KEY_CONFIGS] = configs;
        
        nlohmann::ordered_json typescript;
        typescript[Constants::CONFIG_KEY_TS_ENABLED] = false;
        typescript[Constants::CONFIG_KEY_TS_OUT_DIR] = Constants::DEFAULT_TS_OUT_DIR;
        typescript[Constants::CONFIG_KEY_TS_AUTO_INSTALL] = true;
        j[Constants::CONFIG_KEY_TYPESCRIPT] = typescript;

        std::ofstream file(Constants::FILE_CONFIG);

        if (!file.is_open())
            throw File_Exception(std::string("Failed to create config file: ") + Constants::FILE_CONFIG);

        file << j.dump(4);

        if (!file.good())
            throw File_Exception("Failed to write default config data.");

        file.close();

        Logger::Log(Log_Level::INFO, "Created default config file: '%s'.", Constants::FILE_CONFIG);

        config_.main_file = Constants::DEFAULT_MAIN_FILE;
        config_.use_typescript = false;
        config_.ts_output_dir = Constants::DEFAULT_TS_OUT_DIR;
        config_.ts_auto_install = true;
    }
    catch (const nlohmann::json::exception& e) {
        Logger::Log(Log_Level::ERROR_s, "JSON error while creating default config: '%s'.", e.what());

        Set_Default_Config();
    }
    catch (const File_Exception& e) {
        Logger::Log(Log_Level::ERROR_s, "'%s'.", e.what());

        Set_Default_Config();
    }
    catch (const std::exception& e) {
        Logger::Log(Log_Level::ERROR_s, "Error creating default config: '%s'.", e.what());

        Set_Default_Config();
    }
}

void File_Manager::Set_Default_Config() {
    config_.main_file = Constants::DEFAULT_MAIN_FILE;
    config_.use_typescript = false;
    config_.ts_output_dir = Constants::DEFAULT_TS_OUT_DIR;
    config_.ts_auto_install = true;
}

bool File_Manager::Validate_Main_File(const std::string& path) const {
    try {
        if (path.empty())
            throw Config_Exception("Main file path is empty.");

        size_t path_len = path.length();
        size_t js_ext_len = strlen(Constants::JS_EXTENSION);
        size_t ts_ext_len = strlen(Constants::TS_EXTENSION);

        bool is_js = path_len >= js_ext_len && path.substr(path_len - js_ext_len) == Constants::JS_EXTENSION;
        bool is_ts = path_len >= ts_ext_len && path.substr(path_len - ts_ext_len) == Constants::TS_EXTENSION;

        if (!is_js && !is_ts)
            throw Config_Exception(std::string("Main file must end with '") + Constants::JS_EXTENSION + "' or '" + Constants::TS_EXTENSION + "' extension: " + path);

        if (!fs::exists(path))
            throw Config_Exception("Main file not found: " + path);

        return true;
    }
    catch (const Config_Exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "'%s'.", e.what()), false);
    }
    catch (const fs::filesystem_error& e) {
        return (Logger::Log(Log_Level::ERROR_s, "Filesystem error validating main file: '%s'.", e.what()), false);
    }
}

void File_Manager::Create_Default_TSConfig() const {
    try {
        nlohmann::ordered_json tsconfig;
        nlohmann::ordered_json compiler_options;

        compiler_options["target"] = Constants::TSCONFIG_TARGET;
        compiler_options["module"] = Constants::TSCONFIG_MODULE;
        compiler_options["outDir"] = config_.ts_output_dir.empty() ? Constants::DEFAULT_TS_OUT_DIR : config_.ts_output_dir;
        compiler_options["rootDir"] = Constants::TSCONFIG_ROOT_DIR;
        compiler_options["esModuleInterop"] = true;
        compiler_options["skipLibCheck"] = true;
        compiler_options["strict"] = true;
        compiler_options["resolveJsonModule"] = true;
        compiler_options["moduleResolution"] = Constants::TSCONFIG_MODULE_RESOLUTION;

        tsconfig["compilerOptions"] = compiler_options;
        tsconfig["include"] = nlohmann::json::array({ Constants::TSCONFIG_INCLUDE_PATTERN });
        tsconfig["exclude"] = nlohmann::json::array({ Constants::TSCONFIG_EXCLUDE_NODE_MODULES, config_.ts_output_dir });

        std::ofstream file(Constants::FILE_TSCONFIG);

        if (!file.is_open())
            throw File_Exception(std::string("Failed to create 'tsconfig.json' file: ") + Constants::FILE_TSCONFIG);

        file << tsconfig.dump(2);

        if (!file.good())
            throw File_Exception("Failed to write to 'tsconfig.json'.");

        file.close();

        Logger::Log(Log_Level::INFO, "Created default 'tsconfig.json' file.");
    }
    catch (const nlohmann::json::exception& e) {
        Logger::Log(Log_Level::ERROR_s, "JSON error while creating tsconfig.json: '%s'.", e.what());
    }
    catch (const File_Exception& e) {
        Logger::Log(Log_Level::ERROR_s, "'%s'.", e.what());
    }
    catch (const std::exception& e) {
        Logger::Log(Log_Level::ERROR_s, "Error creating tsconfig.json: '%s'.", e.what());
    }
}

bool File_Manager::Update_TSConfig_Output_Dir(const std::string& new_output_dir) const {
    try {
        if (!fs::exists(Constants::FILE_TSCONFIG))
            return (Logger::Log(Log_Level::WARNING, "'tsconfig.json' not found, cannot update 'outDir'."), false);
        
        std::ifstream file(Constants::FILE_TSCONFIG);

        if (!file.is_open())
            throw File_Exception(std::string("Failed to open 'tsconfig.json' for reading: ") + Constants::FILE_TSCONFIG);
        
        nlohmann::ordered_json tsconfig;

        try {
            file >> tsconfig;
        }
        catch (const nlohmann::json::exception& e) {
            file.close();

            throw File_Exception(std::string("Failed to parse 'tsconfig.json': ") + e.what());
        }

        file.close();
        
        if (!tsconfig.contains("compilerOptions"))
            tsconfig["compilerOptions"] = nlohmann::ordered_json::object();
        
        std::string current_out_dir = "";

        if (tsconfig["compilerOptions"].contains("outDir") && tsconfig["compilerOptions"]["outDir"].is_string())
            current_out_dir = tsconfig["compilerOptions"]["outDir"].get<std::string>();
        
        if (current_out_dir == new_output_dir)
            return true;

        tsconfig["compilerOptions"]["outDir"] = new_output_dir;
        
        if (tsconfig.contains("exclude") && tsconfig["exclude"].is_array()) {
            nlohmann::json new_exclude = nlohmann::json::array();
            bool old_dir_found = false;
            
            for (const auto& item : tsconfig["exclude"]) {
                if (item.is_string()) {
                    std::string exclude_item = item.get<std::string>();

                    if (exclude_item == current_out_dir) {
                        new_exclude.push_back(new_output_dir);
                        old_dir_found = true;
                    }
                    else
                        new_exclude.push_back(exclude_item);
                }
            }
            
            if (!old_dir_found)
                new_exclude.push_back(new_output_dir);

            tsconfig["exclude"] = new_exclude;
        }
        
        std::ofstream out_file(Constants::FILE_TSCONFIG);

        if (!out_file.is_open())
            throw File_Exception(std::string("Failed to open 'tsconfig.json' for writing: ") + Constants::FILE_TSCONFIG);
        
        out_file << tsconfig.dump(2);

        if (!out_file.good())
            throw File_Exception("Failed to write updated 'tsconfig.json'.");
        
        out_file.close();
        
        Logger::Log(Log_Level::INFO, "Updated 'tsconfig.json' 'outDir' from '%s' to '%s'.", current_out_dir.c_str(), new_output_dir.c_str());
        
        return true;
    }
    catch (const File_Exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "'%s'.", e.what()), false);
    }
    catch (const nlohmann::json::exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "JSON error updating 'tsconfig.json': '%s'.", e.what()), false);
    }
    catch (const std::exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "Error updating 'tsconfig.json': '%s'.", e.what()), false);
    }
}

bool File_Manager::Load_Config() {
    try {
        if (!fs::exists(Constants::FILE_CONFIG))
            return (Create_Default_Config(), true);

        std::ifstream file(Constants::FILE_CONFIG);

        if (!file.is_open())
            throw File_Exception(std::string("Failed to open config file: ") + Constants::FILE_CONFIG);

        nlohmann::ordered_json j;

        try {
            file >> j;
        }
        catch (const nlohmann::json::exception& e) {
            file.close();

            throw Config_Exception(std::string("Failed to parse config JSON: ") + e.what());
        }

        file.close();

        bool save_needed = false;

        Ensure_JSON_Section(j, Constants::CONFIG_KEY_CONFIGS, save_needed);
        nlohmann::ordered_json& configs = j[Constants::CONFIG_KEY_CONFIGS];
        
        Load_JSON_Field(configs, Constants::CONFIG_KEY_MAIN_FILE, config_.main_file, std::string(Constants::DEFAULT_MAIN_FILE), configs, save_needed);
        
        Ensure_JSON_Section(j, Constants::CONFIG_KEY_TYPESCRIPT, save_needed);
        nlohmann::ordered_json& typescript = j[Constants::CONFIG_KEY_TYPESCRIPT];
        
        Load_JSON_Field(typescript, Constants::CONFIG_KEY_TS_ENABLED, config_.use_typescript, false, typescript, save_needed);
        Load_JSON_Field(typescript, Constants::CONFIG_KEY_TS_OUT_DIR, config_.ts_output_dir, std::string(Constants::DEFAULT_TS_OUT_DIR), typescript, save_needed);
        Load_JSON_Field(typescript, Constants::CONFIG_KEY_TS_AUTO_INSTALL, config_.ts_auto_install, true, typescript, save_needed);

        if (save_needed)
            Save_JSON_Config(j, Constants::FILE_CONFIG);

        if (!Validate_Main_File(config_.main_file))
            throw Config_Exception("Invalid main file path in config.");
        
        if (config_.use_typescript) {
            if (!fs::exists(Constants::FILE_TSCONFIG)) {
                Logger::Log(Log_Level::INFO, "TypeScript enabled but 'tsconfig.json' not found. Creating default 'tsconfig.json'...");

                Create_Default_TSConfig();
            }
            else
                Update_TSConfig_Output_Dir(config_.ts_output_dir);

            try {
                std::string output_dir = config_.ts_output_dir.empty() ? Constants::DEFAULT_TS_OUT_DIR : config_.ts_output_dir;
                    
                if (!fs::exists(output_dir)) {
                    if (!fs::create_directories(output_dir))
                        throw File_Exception("Failed to create TypeScript output directory: " + output_dir);

                    Logger::Log(Log_Level::INFO, "Created TypeScript output directory: '%s'.", output_dir.c_str());
                }
            }
            catch (const fs::filesystem_error& e) {
                Logger::Log(Log_Level::WARNING, "Failed to create TypeScript output directory: '%s'.", e.what());
            }
        }

        return true;
    }
    catch (const Plugin_Exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "'%s'.", e.what()), false);
    }
    catch (const std::exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "Unexpected error loading config: '%s'.", e.what()), false);
    }
}
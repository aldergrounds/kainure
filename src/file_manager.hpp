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

#include <string>

class File_Manager {
    public:
        struct Config {
            std::string main_file;
            bool use_typescript = false;
            std::string ts_output_dir;
            bool ts_auto_install = true;
        };

        static File_Manager& Instance();

        bool Initialize();
        const Config& Get_Config() const;
        bool Validate_Main_File(const std::string& path) const;
        bool Update_TSConfig_Output_Dir(const std::string& new_output_dir) const;

    private:
        File_Manager() = default;
        ~File_Manager() = default;
        File_Manager(const File_Manager&) = delete;
        File_Manager& operator=(const File_Manager&) = delete;

        bool Create_Directories();
        bool Load_Config();
        void Create_Default_Config();
        void Create_Default_TSConfig() const;
        void Set_Default_Config();

        Config config_;
};
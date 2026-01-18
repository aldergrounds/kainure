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

#define SAMP_SDK_IMPLEMENTATION
#define SAMP_SDK_WANT_PROCESS_TICK
#include "sdk/samp_sdk.hpp"
//
#include "constants.hpp"
#include "encoding_converter.hpp"
#include "runtime_manager.hpp"
#include "event_dispatcher.hpp"
#include "natives.hpp"
#include "publics.hpp"
#include "file_manager.hpp"
#include "logger.hpp"
#include "exceptions.hpp"
#include "native_hooks.hpp"

unsigned int GetSupportFlags() {
    return SUPPORTS_VERSION;
}

bool OnLoad() {
    try {
        Logger::Log(Log_Level::INFO, "Starting plugin initialization...");

        if (!File_Manager::Instance().Initialize())
            return (Logger::Log(Log_Level::ERROR_s, "Failed to initialize 'File Manager'."), false);

        const auto& config = File_Manager::Instance().Get_Config();

        if (!File_Manager::Instance().Validate_Main_File(config.main_file))
            return (Logger::Log(Log_Level::ERROR_s, "Main file validation failed. Aborting load to prevent crash."), false);

        Encoding_Converter::Instance().Initialize(config.encoding_enabled, config.encoding_target);

        Samp_SDK::Detail::Get_Public_Handler() = Publics::Handler;
        Samp_SDK::Detail::Get_Has_Public_Handler() = Publics::Has_Handler;

        if (!Runtime_Manager::Instance().Initialize())
            return (Logger::Log(Log_Level::ERROR_s, "Failed to initialize 'Runtime Manager'."), false);

        Event_Dispatcher::Instance().Generate_Hook_Bindings();

        if (!Runtime_Manager::Instance().Execute_Bootstrap_Script()) {
            Logger::Log(Log_Level::ERROR_s, "Failed to execute bootstrap scripts.");
            Logger::Log(Log_Level::ERROR_s, "If the bootstrap fails because the module cannot be found, the server will crash shortly afterward. Fix the issue.");

            Runtime_Manager::Instance().Shutdown();

            return false;
        }

        Event_Dispatcher::Instance().Generate_Public_Bindings();

        Logger::Log(Log_Level::INFO, "Plugin loaded successfully.");

        return true;
    }
    catch (const Plugin_Exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "Plugin exception during load: '%s'.", e.what()), false);
    }
    catch (const std::exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "Unexpected exception during load: '%s'.", e.what()), false);
    }
}

void OnUnload() {
    try {
        Logger::Log(Log_Level::INFO, "Unloading plugin...");

        Native_Hooks::Instance().Clear();
        Natives::Clear_Bindings();
        Runtime_Manager::Instance().Shutdown();

        Logger::Log(Log_Level::INFO, "Plugin unloaded successfully.");
    }
    catch (const std::exception& e) {
        Logger::Log(Log_Level::ERROR_s, "Exception during unload: '%s'.", e.what());
    }
}

void OnProcessTick() {
    try {
        Runtime_Manager::Instance().Process_Tick();
    }
    catch (const std::exception& e) {
        Logger::Log(Log_Level::ERROR_s, "Exception in process tick: '%s'.", e.what());
    }
}

Plugin_Public(OnGameModeInit) {
    try {
        Event_Dispatcher::Instance().Generate_Native_Bindings();
        Event_Dispatcher::Instance().Dispatch_Event(Constants::EVENT_GAMEMODE_INIT);

        return PLUGIN_PUBLIC_CONTINUE;
    }
    catch (const std::exception& e) {
        return (Logger::Log(Log_Level::ERROR_s, "Exception in OnGameModeInit: '%s'.", e.what()), PLUGIN_PUBLIC_CONTINUE);
    }
}
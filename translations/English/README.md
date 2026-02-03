<div align="center">

# **Kainure**

![C++](https://img.shields.io/badge/C%2B%2B-20-00599C?logo=cplusplus&style=for-the-badge)
![Node.js](https://img.shields.io/badge/Node.js-v22.21.1-339933?logo=node.js&style=for-the-badge)
![V8](https://img.shields.io/badge/V8-12.4-4B8BF5?logo=v8&style=for-the-badge)
![libuv](https://img.shields.io/badge/libuv-v1.48.0-blue?logo=libuv&style=for-the-badge)
![JavaScript](https://img.shields.io/badge/JavaScript-ES2023-F7DF1E?logo=javascript&style=for-the-badge)
![TypeScript](https://img.shields.io/badge/TypeScript-5.x-3178C6?logo=typescript&style=for-the-badge)
![License](https://img.shields.io/badge/License-Apache_2.0-blue?style=for-the-badge)

**Framework for modern SA-MP development with JavaScript and TypeScript.**

</div>

## Languages

- Português: [README](../../)
- Deutsch: [README](../Deutsch/README.md)
- Español: [README](../Espanol/README.md)
- Français: [README](../Francais/README.md)
- Italiano: [README](../Italiano/README.md)
- Polski: [README](../Polski/README.md)
- Русский: [README](../Русский/README.md)
- Svenska: [README](../Svenska/README.md)
- Türkçe: [README](../Turkce/README.md)

## Table of Contents

- [**Kainure**](#kainure)
  - [Languages](#languages)
  - [Table of Contents](#table-of-contents)
  - [What is Kainure?](#what-is-kainure)
  - [How to Install and Use](#how-to-install-and-use)
    - [1. Download](#1-download)
    - [2. File Structure](#2-file-structure)
    - [3. Configuration (config.json)](#3-configuration-configjson)
  - [API and Features](#api-and-features)
    - [Philosophy: Globals and No Hassle](#philosophy-globals-and-no-hassle)
    - [Main Module: `api.js`](#main-module-apijs)
      - [`Float`](#float)
      - [`Ref`](#ref)
      - [`Public`](#public)
      - [`Call_Public`](#call_public)
      - [`Native`](#native)
      - [`Native_Hook`](#native_hook)
      - [`Include_Storage`](#include_storage)
    - [Commands Module: `commands.js`](#commands-module-commandsjs)
      - [`Command`](#command)
      - [`Command_Params`](#command_params)
      - [`Alias_Command`](#alias_command)
      - [`Call_Command`](#call_command)
      - [`External_Commands`](#external_commands)
    - [Constants and Types](#constants-and-types)
      - [`samp_constants.js`](#samp_constantsjs)
      - [`types/` Directory](#types-directory)
  - [Official Gamemode](#official-gamemode)
  - [Compilation](#compilation)
    - [Windows](#windows)
    - [Linux](#linux)
  - [License](#license)
    - [Terms and Conditions of Use](#terms-and-conditions-of-use)
      - [1. Granted Permissions](#1-granted-permissions)
      - [2. Mandatory Conditions](#2-mandatory-conditions)
      - [3. Restrictions and Limitations](#3-restrictions-and-limitations)
      - [4. Intellectual Property](#4-intellectual-property)
      - [5. Disclaimer of Warranties and Limitation of Liability](#5-disclaimer-of-warranties-and-limitation-of-liability)

## What is Kainure?

Kainure is a framework that serves as a bridge between the traditional San Andreas: Multiplayer (SA-MP) and modern software development. It allows you to write all your gamemode logic using **JavaScript** or **TypeScript**, opening a universe of possibilities with current development tools, packages, and practices.

The plugin integrates a **Node.js** environment directly into your SA-MP server. The specific Node.js version used is **v22.21.1**, which brings the power of high-performance components:
- **V8 Engine:** The same high-performance JavaScript engine from **Google Chrome**, ensuring your code runs with impressive speed and efficiency.
- **libuv:** A support library that handles I/O (input and output operations) asynchronously, allowing your server to handle multiple operations simultaneously without freezing, ideal for a dynamic game environment.

With Kainure, you can leave aside Pawn's limitations and embrace a robust ecosystem to create gamemodes that are more complex, organized, and easier to maintain.

## How to Install and Use

### 1. Download

The latest version of Kainure, including the plugin and all necessary files, can be found on the project's [**Releases**](https://github.com/aldergrounds/kainure/releases) page.

### 2. File Structure

After downloading, you need to organize the files in the root of your SA-MP server as follows:

```
/ (Gamemode Root)
├── plugins/
│   └── Kainure.dll  (or Kainure.so on Linux)
├── Kainure/
│   ├── core/
│   ├── types/
│   ├── kainure.js
│   └── config.json (will be created on first startup if it doesn't exist)
└── libnode.dll (or libnode.so on Linux)
```

**Important:**
- The `Kainure` plugin must be added to your `server.cfg` file.
- The `libnode` library (`.dll` or `.so`) must be in the server **root**, along with the `samp-server` executable (or `samp03svr` on Linux).

### 3. Configuration (config.json)

Inside the `Kainure/` folder, you will find the `config.json` file. If it does not exist, the plugin will create it automatically the first time it is run. This file controls the behavior of the framework.

```json
{
    "configs": {
        "main_file": "./main.js",
        "encoding": {
            "enabled": false,
            "target": 65001
        }
    },
    "typescript": {
        "enabled": false,
        "output_dir": "./dist"
    }
}
```

- **`main_file`**: This is the entry point of your gamemode. Kainure will start executing the code from this file. By default, it can be `.js` or `.ts` (if TypeScript is enabled). The `./` path represents the root of your gamemode.

- **`encoding.enabled`**: Defines whether Kainure should convert strings between the UTF-8 format (JavaScript default) and your server's encoding. If set to `false`, the framework will use the default `65001` (UTF-8).

- **`encoding.target`**: Defines the target encoding.
   - **Windows**: Use of the **Codepage number** is mandatory (e.g., `1252`).
   - **Linux**: Use of the **Codepage name** is mandatory (e.g., `WINDOWS-1252`).

- **`typescript.enabled`**: Change to `true` if you want to write your code in TypeScript. Kainure will automatically transpile `.ts` files to `.js`.
- **`typescript.output_dir`**: Defines the directory where the transpiled JavaScript files will be saved.

## API and Features

### Philosophy: Globals and No Hassle

Kainure was designed to be intuitive. You **do not need to import or `require`** any of the main modules (`api.js`, `commands.js`, etc.) in your files. The framework automatically loads and injects all its features into the global scope (`globalThis`), making them immediately accessible anywhere in your code.

### Main Module: `api.js`

This module contains the essential functions for interacting with the SA-MP server.

#### `Float`

Ensures a number is treated as a `float`, even if it could be interpreted as an integer. This is crucial for SA-MP natives that require floats.

**When to use?** Use `Float()` only when the value is dynamic and unknown at compile time (e.g., coming from a command or another function). Its manual use is relevant only when passing values to `Native.` and `Call_Public.`.

> [!IMPORTANT]
> If you write an explicit value in the code, like `50.0`, Kainure's code preprocessor **automatically** applies `Float` treatment before sending it to the V8 engine. Therefore, manual use is only necessary in specific cases.

```javascript
Command('sethealth', (playerid, params) => {
    const health = { value: 0.1 }; // "0.1" so JavaScript doesn't optimize to integer.

    if (!Command_Params(params, "f", health))
        return Native.SendClientMessage(playerid, -1, "Usage: /sethealth [health]");

    // `health.value` is unknown, so we use Float() to ensure correct typing.
    Native.SetPlayerHealth(playerid, Float(health.value));
});
```

#### `Ref`

Marks a variable to receive a value by reference from a native function.

**How it works?**
- To **receive** a value from a native (output parameters), use the `.$` syntax when passing the variable.
- To **pass** the value contained in the variable to a native, use it normally.
- The initial value passed to `Ref()` determines the expected data type.

**Initialization:**
- **`int`**: `Ref()` or `Ref(0)`
- **`float`**: `Ref(0.1)` (or any float that doesn't end in absolute zero, to avoid optimization to integer by JavaScript)
- **`string`**: `Ref("")`
- **`bool`**: `Ref(true)` or `Ref(false)`

```javascript
Command('health', (playerid) => {
    const health = Ref(0.1); // We expect to receive a float.

    // We use health.$ so the native fills the variable with the player's health.
    Native.GetPlayerHealth(playerid, health.$);

    // We use only `health` to read the value. The Ref is automatically converted to its value.
    Native.SendClientMessage(playerid, -1, `Your health: ${health}`);
});
```

#### `Public`

Declares a SA-MP callback (public), allowing your code to react to game events.

**Parameter Signatures:** For Kainure to convert types correctly, it is mandatory to provide a signature as the second parameter if the callback has arguments.
- **`i`**: Integer
- **`f`**: Float
- **`s`**: String
- **`b`**: Boolean

**Validations and Errors:** Kainure prevents execution if:
1. The function has parameters, but no signature is provided.
2. The number of characters in the signature is different from the number of parameters defined in the function.
3. The signature contains invalid types (different from `i, f, s, b`).

**Return Values:**
- `return 1;` or `return true;`: Allows the callback to continue being executed in other scripts (if any). This is the default behavior if nothing is returned.
- `return 0;` or `return false;`: Prevents the callback from being executed in other scripts.

```javascript
// Simple public without parameters
Public('OnGameModeInit', () => {
    // Logic
    return true;
});

// Public with signature
Public('OnPlayerText', 'is', (playerid, text) => {
    // Logic
    return true;
});
```

#### `Call_Public`

Calls any public, whether from the gamemode itself, a filterscript, or a custom public created with `Public()`.

```javascript
Command('callpublic', (playerid) => {
    // Calls the OnPlayerSpawn public for the player who used the command.
    Call_Public.OnPlayerSpawn(playerid);
});
```

#### `Native`

Invokes a SA-MP native function or one from any loaded plugin. Simply add the `Native.` prefix to the function name.

```javascript
Public('OnPlayerConnect', (playerid) => {
    Native.SendClientMessage(playerid, -1, "Welcome to the server.");
});
```

#### `Native_Hook`

Intercepts a native function call, allowing you to modify its behavior or add extra logic.

**Execution Order:** Hooks follow a **LIFO (Last-In, First-Out)** pattern. The last hook created for a native will be the first to run.

**Return Values:**
- `return 1;` or `return true;`: Allows the native execution to continue (calling the next hook in the stack or the original function).
- `return 0;` or `return false;`: Blocks the execution of the original native.

```javascript
Native_Hook('SetPlayerHealth', (playerid, health) => {
    console.log(`[Hook]: User ${playerid}'s health was set to ${health}.`);
    
    // Allows the original SetPlayerHealth native to execute.
    return true; 
});
```

#### `Include_Storage`

Provides a unique and safe directory path so `includes` (libraries) can store their own data, avoiding conflicts. The folder will always be created inside `Kainure/includes_storage/`.

```javascript
// Returns the path: "Kainure/includes_storage/MyInclude"
const my_data_path = Include_Storage("MyInclude"); 
```

### Commands Module: `commands.js`

This module offers a complete system for creating and managing commands.

#### `Command`

Registers a new command on the server. The callback function can receive up to two parameters: `playerid` and `params`. Both are optional.

```javascript
// Simple command without parameters
Command('pos', (playerid) => {
    const x = Ref(0.1), y = Ref(0.1), z = Ref(0.1);

    Native.GetPlayerPos(playerid, x.$, y.$, z.$);
    Native.SendClientMessage(playerid, -1, `Position: ${x}, ${y}, ${z}`);
});
```

#### `Command_Params`

Processes and extracts parameters from a command string, similar to `sscanf`.

**How it works?**
- The variable that will receive the value must be an object with a `value` property (e.g., `{ value: 0 }`).
- The initial value of the `value` property defines the expected data type.
- After the function, the extracted value will be accessible via `.value`.

```javascript
Command('givemoney', (playerid, params) => {
    const giveid = { value: 0 }; // Expects an integer
    const amount = { value: 0 }; // Expects an integer

    if (!Command_Params(params, "ii", giveid, amount))
        return Native.SendClientMessage(playerid, -1, "Correct usage: /givemoney [ID] [Amount]");

    Native.GivePlayerMoney(giveid.value, amount.value);
    // Note the use of .value to access processed values.
});
```

#### `Alias_Command`

Creates one or more aliases (alternative names) for an existing command.

```javascript
// Creates the main command
Command('showlife', (playerid) => { /* ... */ });

// Creates aliases for the command 'showlife'
Alias_Command('showlife', 'life', 'hp', 'health');
```
Now, `/showlife`, `/life`, `/hp` and `/health` will execute the same code.

#### `Call_Command`

Calls a command programmatically from your code.

```javascript
// Example: Forces player 0 to execute /givemoney 1 500, meaning they will give 500 money to player 1.
Call_Command("givemoney", "1 500", 0 /* or playerid */);

// The playerid at the end is optional. However, this is illogical since commands are player-based.
Call_Command("announcement", "The server will restart in 1 minute!");
```

#### `External_Commands`

This function is used to register commands that **were not created in the Kainure environment**, but exist in other scripts.

By registering a command as external, Kainure ensures that global command callbacks (`OnPlayerCommandReceived` and `OnPlayerCommandPerformed`) are triggered correctly for these commands, integrating them into the framework's flow.

```javascript
// Registering commands that exist in a filterscript
External_Commands("login", "register", "help");
```

### Constants and Types

#### `samp_constants.js`

This file contains all standard SA-MP constants and definitions (like `MAX_PLAYERS`, `WEAPON_DEAGLE`, etc.). It is loaded globally, so you can use these constants directly in your code without needing to declare them.

#### `types/` Directory

This directory stores type definition files (`.d.ts`) for the entire Kainure API. If you are using TypeScript, these files will provide autocomplete, type checking, and a much richer and safer development experience in your code editor.

## Official Gamemode

To accelerate your learning and see Kainure in action, **AlderGrounds** maintains a complete, open-source gamemode built entirely with this framework. It serves as a key piece of living documentation and the best place to find practical examples.

If you have questions about how to structure your code, use the API, or implement complex features, this repository is your main source of reference.

➡️ **Explore the Official Gamemode:** [AlderGrounds Gamemode](https://github.com/aldergrounds/gamemode)

## Compilation

If you wish to compile Kainure from source code, follow the instructions for your operating system.

### Windows

1. **Requirements**:
   - [Visual Studio](https://visualstudio.microsoft.com/) with the "Desktop development with C++" workload installed.
2. **Steps**:
   - Navigate to the `build/Windows/` directory.
   - Open the appropriate solution file for your Visual Studio version:
      - `Kainure.sln`: For Visual Studio 2022 or lower.
      - `Kainure.slnx`: For Visual Studio 2026.
   - Inside Visual Studio, make sure the build configuration is set to **Release** and the platform to **x86**.
   - Build the solution (usually by pressing `Ctrl+Shift+B` or via `Build > Build Solution` menu).
   - The compiled `Kainure.dll` will be found in the `compiled/Windows/` folder.

### Linux

1. **Requirements**:
   - [Docker](https://www.docker.com/) installed and running.
2. **Steps**:
   - The process is automated by a script. Just run the following command from the project root:
      ```bash
      ./build/Linux/compile.sh
      ```
   - The script will:
      1. Build a Docker image (`kainure-builder-x86`) with all necessary dependencies (Ubuntu 18.04, g++-9, CMake).
      2. Run a container from the image to compile the project.
      3. Copy the resulting `Kainure.so` to the `compiled/Linux/` folder.
      4. Clean up the created Docker image.

## License

Copyright © **AlderGrounds**

This software is licensed under the terms of the Apache License, Version 2.0 ("License"); you may not use this software except in compliance with the License. A copy of the License can be obtained at: [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0)

### Terms and Conditions of Use

#### 1. Granted Permissions

The present license grants, free of charge, to any person obtaining a copy of this software and associated documentation files, the following rights:
- Use, copy, modify, and distribute the software in any medium or format, for any purpose, commercial or non-commercial
- Merge, publish, distribute, sublicense, and/or sell copies of the software
- Permit persons to whom the software is furnished to do the same

#### 2. Mandatory Conditions

All distributions of the software or derivative works must:
- Include a complete copy of this license
- Clearly indicate any modifications made to the original source code
- Preserve all copyright, patent, trademark, and attribution notices
- Provide adequate documentation of implemented changes
- Maintain the license and warranty notice in all copies

#### 3. Restrictions and Limitations

- This license does not grant permission to use trademarks, logos, or trade names of **AlderGrounds**
- Contributions to the source code must be licensed under the same terms as this license
- The use of contributor names to endorse or promote products derived from this software requires specific prior permission

#### 4. Intellectual Property

The software and all associated documentation are protected by copyright laws and international treaties. **AlderGrounds** retains all rights, titles, and interests not expressly granted by this license.

#### 5. Disclaimer of Warranties and Limitation of Liability

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.

IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

---

For detailed information about the Apache License 2.0, see: http://www.apache.org/licenses/LICENSE-2.0
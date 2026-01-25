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

class Command_Processor {
    constructor() {
        this.command_map = new Map();
        this.external_commands = new Set();
        this.has_on_command_received = false;
        this.has_on_command_performed = false;
    }

    Initialize_Callbacks() {
        this.has_on_command_received = Kainure_Has_Listeners('OnPlayerCommandReceived');
        this.has_on_command_performed = Kainure_Has_Listeners('OnPlayerCommandPerformed');
    }

    Register(name, callback) {
        if (typeof name !== 'string' || name.length === 0) {
            console.log("Command Processor | Register: Command name must be a non-empty string.");

            return;
        }

        if (typeof callback !== 'function') {
            console.log(`Command Processor | Register: Handler for command '${name}' must be a function.`);

            return;
        }
        
        const normalized_name = name.startsWith('/') ? name.substring(1).toLowerCase() : name.toLowerCase();

        if (this.command_map.has(normalized_name))
            console.log(`Command Processor | Register: Command '${normalized_name}' is being overwritten.`);

        this.command_map.set(normalized_name, callback);
    }

    Register_External(...command_names) {
        for (const name of command_names) {
            if (typeof name !== 'string' || name.length === 0) {
                console.log("Command Processor | Register_External: Command name must be a non-empty string.");

                continue;
            }

            const normalized_name = name.startsWith('/') ? name.substring(1).toLowerCase() : name.toLowerCase();

            if (this.command_map.has(normalized_name)) {
                console.log(`Command Processor | Register_External: Command '${normalized_name}' is already registered in JS. External registration ignored.`);

                continue;
            }

            this.external_commands.add(normalized_name);
        }
    }

    Aliases(original_command_name, ...alias_names) {
        const normalized_original = original_command_name.toLowerCase();
        const original_handler = this.command_map.get(normalized_original);

        if (!original_handler) {
            console.log(`Command Processor | Aliases: Original command '${normalized_original}' not found.`);

            return;
        }

        for (const alias of alias_names) {
            if (typeof alias !== 'string' || alias.length === 0) {
                console.log("Command Processor | Aliases: Attempt to register an invalid alias.");

                continue;
            }
            
            const normalized_alias = alias.toLowerCase();

            if (this.command_map.has(normalized_alias))
                console.log(`Command Processor | Aliases: Alias '${normalized_alias}' already exists and is being overwritten.`);

            this.command_map.set(normalized_alias, original_handler);
        }
    }

    Call(command_name, params, playerid) {
        if (typeof command_name !== 'string' || command_name.length === 0) {
            console.log("Command Processor | Call: Command name must be a string.");

            return false;
        }
        
        const normalized_name = command_name.toLowerCase();
        const handler = this.command_map.get(normalized_name);

        if (handler) {
            handler(playerid, params || "");

            return true;
        }

        return false;
    }

    Is_External_Command(command_name) {
        return this.external_commands.has(command_name.toLowerCase());
    }

    Process(playerid, cmdtext) {
        if (!cmdtext || !cmdtext.startsWith('/'))
            return 1;

        this.Initialize_Callbacks();

        const text = cmdtext.substring(1);
        const parts = text.split(' ');
        const command_name = parts[0].toLowerCase();
        
        const command_exists = this.command_map.has(command_name);
        const is_external = this.Is_External_Command(command_name);

        if (this.has_on_command_received) {
            const received_result = Kainure_Emit_Event('OnPlayerCommandReceived', playerid, cmdtext);
            
            if (received_result === 0)
                return 0;
        }

        if (!command_exists) {
            if (is_external) {
                if (this.has_on_command_performed)
                    Kainure_Emit_Event('OnPlayerCommandPerformed', playerid, cmdtext, 1);

                return 0;
            }

            if (this.has_on_command_performed) {
                const performed_result = Kainure_Emit_Event('OnPlayerCommandPerformed', playerid, cmdtext, 0);
                
                if (performed_result !== undefined)
                    return performed_result;
            }

            return 1;
        }

        const params = parts.slice(1).join(' ');
        const success = this.Call(command_name, params, playerid);

        if (this.has_on_command_performed) {
            const performed_result = Kainure_Emit_Event('OnPlayerCommandPerformed', playerid, cmdtext, success ? 1 : 0);
            
            if (performed_result !== undefined)
                return performed_result;
        }

        return 1;
    }
}

class Param_Parser {
    constructor() {
        this.type_handlers = this.Initialize_Type_Handlers();
    }

    Initialize_Type_Handlers() {
        return {
            'i': this.Parse_Integer.bind(this),
            'd': this.Parse_Integer.bind(this),
            'f': this.Parse_Float.bind(this),
            's': this.Parse_String.bind(this),
            'z': this.Parse_Optional_String.bind(this),
            'S': this.Parse_Full_String.bind(this),
            'b': this.Parse_Boolean.bind(this),
            'h': this.Parse_Hexadecimal.bind(this),
            'c': this.Parse_Character.bind(this),
            'r': this.Parse_Player.bind(this),
            'u': this.Parse_Player.bind(this),
            'array': this.Parse_Array.bind(this)
        };
    }

    Tokenize_Params(params) {
        const tokens = [];

        let current = '';
        let in_quotes = false;
        let quote_char = null;
        
        for (let i = 0; i < params.length; i++) {
            const char = params[i];
            
            if ((char === '"' || char === "'") && (quote_char === null || quote_char === char)) {
                if (in_quotes && quote_char === char) {
                    in_quotes = false;
                    quote_char = null;
                }
                else if (!in_quotes) {
                    in_quotes = true;
                    quote_char = char;
                }

                continue;
            }
            
            if (char === ' ' && !in_quotes) {
                if (current.length > 0) {
                    tokens.push(current);
                    current = '';
                }
            }
            else
                current += char;
        }
        
        if (current.length > 0)
            tokens.push(current);

        return tokens;
    }

    Parse_Format_Spec(format, start_index) {
        let index = start_index;
        let type = format[index];
        let optional = false;
        let default_value = undefined;
        let array_size = 0;
        let array_type = null;

        if (type === 'a') {
            const array_info = this.Parse_Array_Format(format, index);

            if (!array_info.success) {
                return {
                    success: false,
                    error: array_info.error
                };
            }
            
            array_type = array_info.array_type;
            array_size = array_info.array_size;
            index = array_info.next_index;
            type = 'array';
        }
        else
            index++;

        if (index < format.length && format[index] === '(') {
            const default_info = this.Parse_Default_Value(format, index);

            default_value = default_info.value;
            index = default_info.next_index;
        }

        if (index < format.length && format[index] === '?') {
            optional = true;
            index++;
        }

        return {
            success: true,
            type,
            optional,
            default_value,
            array_size,
            array_type,
            next_index: index
        };
    }

    Parse_Array_Format(format, start_index) {
        let index = start_index + 1;
        
        if (index >= format.length || format[index] !== '<') {
            return {
                success: false,
                error: "Array format must be 'a<type>[size]'"
            };
        }
        
        index++;

        const array_type = format[index];

        index++;
        
        if (format[index] !== '>') {
            return {
                success: false,
                error: "Array format must be 'a<type>[size]'"
            };
        }
        
        index++;
        
        if (format[index] !== '[') {
            return {
                success: false,
                error: "Array format must be 'a<type>[size]'"
            };
        }
        
        index++;
        
        let size_str = '';

        while (format[index] !== ']' && index < format.length) {
            size_str += format[index];
            index++;
        }
        
        const array_size = parseInt(size_str, 10);

        index++;
        
        return {
            success: true,
            array_type,
            array_size,
            next_index: index
        };
    }

    Parse_Default_Value(format, start_index) {
        let index = start_index + 1;
        let value_str = '';
        
        while (format[index] !== ')' && index < format.length) {
            value_str += format[index];
            index++;
        }
        
        index++;
        
        return {
            value: value_str,
            next_index: index
        };
    }

    Parse_Integer(value, optional) {
        if (!value)
            return optional ? 0 : undefined;
        
        const parsed = parseInt(value, 10);

        return isNaN(parsed) ? undefined : parsed;
    }

    Parse_Float(value, optional) {
        if (!value)
            return optional ? 0.0 : undefined;
        
        const parsed = parseFloat(value);

        return isNaN(parsed) ? undefined : parsed;
    }

    Parse_String(value, optional) {
        return value || (optional ? "" : undefined);
    }

    Parse_Optional_String(value) {
        return value || "";
    }

    Parse_Full_String(tokens, param_index, optional) {
        if (param_index >= tokens.length) {
            return {
                value: optional ? "" : undefined,
                consumed: 0
            };
        }
        
        const full_string = tokens.slice(param_index).join(' ');

        return {
            value: full_string,
            consumed: tokens.length - param_index
        };
    }

    Parse_Boolean(value, optional) {
        if (!value)
            return optional ? false : undefined;
        
        const lower = value.toLowerCase();

        return lower === 'true' || lower === '1' || lower === 'yes' || lower === 'sim';
    }

    Parse_Hexadecimal(value, optional) {
        if (!value)
            return optional ? 0 : undefined;
        
        const parsed = parseInt(value, 16);

        return isNaN(parsed) ? undefined : parsed;
    }

    Parse_Character(value, optional) {
        return value ? value[0] : (optional ? '' : undefined);
    }

    Parse_Player(value, optional) {
        if (!value)
            return optional ? -1 : undefined;
        
        const parsed = parseInt(value, 10);

        if (!isNaN(parsed))
            return parsed;
        
        return value;
    }

    Parse_Array(tokens, param_index, array_size, array_type) {
        const result = [];
        
        for (let i = 0; i < array_size; i++) {
            const token = tokens[param_index + i];

            let value;
            
            const handler = this.type_handlers[array_type];

            if (handler && array_type !== 'array') {
                value = handler(token, false);

                if (value === undefined)
                    value = this.Get_Default_For_Type(array_type);
            }
            else
                value = token || this.Get_Default_For_Type(array_type);
            
            result.push(value);
        }
        
        return {
            value: result,
            consumed: array_size
        };
    }

    Get_Default_For_Type(type) {
        switch (type) {
            case 'i':
            case 'd':
            case 'h':
                return 0;
            case 'f':
                return 0.0;
            case 's':
            case 'z':
            case 'c':
                return "";
            case 'b':
                return false;
            default:
                return null;
        }
    }

    Assign_Variable(variable, value, variables, var_index) {
        if (typeof variable === 'object' && variable !== null && !Array.isArray(variable))
            variable.value = value;
        else
            variables[var_index] = value;
    }

    Parse(params, format, variables) {
        if (!format || format.length === 0) {
            console.log("Param_Parser | Parse: Format string cannot be empty.");

            return false;
        }

        const tokens = this.Tokenize_Params(params);

        let param_index = 0;
        let var_index = 0;
        let format_index = 0;

        while (format_index < format.length) {
            if (var_index >= variables.length) {
                console.log("Param_Parser | Parse: Not enough variables provided for format string.");

                return false;
            }

            const format_spec = this.Parse_Format_Spec(format, format_index);

            if (!format_spec.success) {
                console.log(`Param_Parser | Parse: ${format_spec.error}`);

                return false;
            }

            format_index = format_spec.next_index;

            let current_token = tokens[param_index];
            
            if (current_token === undefined && !format_spec.optional && format_spec.default_value === undefined)
                return false;

            if (current_token === undefined && format_spec.default_value !== undefined)
                current_token = format_spec.default_value;

            let parsed_value;
            let consumed = 1;

            try {
                const handler = this.type_handlers[format_spec.type];
                
                if (!handler) {
                    console.log(`Param_Parser | Parse: Unknown format type: ${format_spec.type}`);

                    return false;
                }

                if (format_spec.type === 'S') {
                    const result = handler(tokens, param_index, format_spec.optional);

                    parsed_value = result.value;
                    consumed = result.consumed;
                }
                else if (format_spec.type === 'array') {
                    const result = handler(tokens, param_index, format_spec.array_size, format_spec.array_type);
                    
                    parsed_value = result.value;
                    consumed = result.consumed;
                }
                else
                    parsed_value = handler(current_token, format_spec.optional);

                if (parsed_value === undefined)
                    return false;

                this.Assign_Variable(variables[var_index], parsed_value, variables, var_index);
            }
            catch (error) {
                console.log(`Param_Parser | Parse: Error parsing parameter: ${error.message}`);

                return false;
            }

            param_index += consumed;
            var_index++;
        }

        return true;
    }
}

const command_processor = new Command_Processor();
const param_parser = new Param_Parser();

globalThis.Command = (name, callback) => {
    command_processor.Register(name, callback);
};

globalThis.Alias_Command = (original_name, ...aliases) => {
    command_processor.Aliases(original_name, ...aliases);
};

globalThis.Call_Command = (name, params, playerid) => {
    return command_processor.Call(name, params, playerid);
};

globalThis.Command_Params = (params, format, ...variables) => {
    return param_parser.Parse(params, format, variables);
}

globalThis.External_Commands = (...command_names) => {
    command_processor.Register_External(...command_names);
};

Public('OnPlayerCommandText', 'is', (playerid, cmdtext) => {
    return command_processor.Process(playerid, cmdtext);
});

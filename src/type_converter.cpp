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

#include <string>
#include <cmath>
#include <memory>
#include <algorithm>
//
#include "sdk/amx/amx_api.hpp"
#include "sdk/events/public_dispatcher.hpp"
#include "sdk/core/platform.hpp"
//
#include "encoding_converter.hpp"
#include "type_converter.hpp"
#include "constants.hpp"

namespace {
    struct V8_String_Cache {
        v8::Eternal<v8::String> value_field;
        v8::Eternal<v8::String> parent_field;
        v8::Eternal<v8::String> kainure_ref_field;
        v8::Eternal<v8::String> kainure_ptr_field;

        bool initialized = false;
        
        SAMP_SDK_FORCE_INLINE void Ensure_Initialized(v8::Isolate* isolate) {
            if (!initialized) {
                value_field.Set(isolate, v8::String::NewFromUtf8(isolate, Constants::VALUE_FIELD, v8::NewStringType::kInternalized).ToLocalChecked());
                parent_field.Set(isolate, v8::String::NewFromUtf8(isolate, Constants::PARENT_FIELD, v8::NewStringType::kInternalized).ToLocalChecked());
                kainure_ref_field.Set(isolate, v8::String::NewFromUtf8(isolate, Constants::KAINURE_REF_FIELD, v8::NewStringType::kInternalized).ToLocalChecked());
                kainure_ptr_field.Set(isolate, v8::String::NewFromUtf8(isolate, Constants::KAINURE_PTR_FIELD, v8::NewStringType::kInternalized).ToLocalChecked());

                initialized = true;
            }
        }
    };
    
    static V8_String_Cache string_cache;
    
    SAMP_SDK_FORCE_INLINE bool Fast_Get_Value(v8::Isolate* isolate, v8::Local<v8::Context> context, v8::Local<v8::Object> obj, v8::Local<v8::Value>& out) {
        v8::MaybeLocal<v8::Value> maybe = obj->Get(context, string_cache.value_field.Get(isolate));

        return maybe.ToLocal(&out);
    }
    
    SAMP_SDK_FORCE_INLINE bool Has_Field(v8::Isolate* isolate, v8::Local<v8::Context> context, v8::Local<v8::Object> obj, v8::Local<v8::String> field) {
        v8::Maybe<bool> has = obj->Has(context, field);

        return has.IsJust() && has.FromJust();
    }
    
    Type_Converter::Conversion_Result Create_Ref_Struct(v8::Local<v8::Object> parent_obj, cell value, AMX* amx, Type_Converter::Ref_Type type) {
        Type_Converter::Conversion_Result result;
        result.memory = std::make_unique<Samp_SDK::Amx_Scoped_Memory>(amx, 1);
        
        if (!result.memory || !result.memory->Is_Valid()) {
            result.value = 0;

            return result;
        }
        
        cell* phys_addr = result.memory->Get_Phys_Addr();
        *phys_addr = value;
        result.value = result.memory->Get_Amx_Addr();
        
        result.update_data.parent = parent_obj;
        result.update_data.phys_addr = phys_addr;
        result.update_data.type = type;

        return result;
    }
}

Type_Converter::Conversion_Result Type_Converter::To_Cell(v8::Isolate* isolate, v8::Local<v8::Context> context, v8::Local<v8::Value> value, AMX* amx) {
    string_cache.Ensure_Initialized(isolate);
    
    Conversion_Result result;
    result.value = 0;
    
    if (value->IsInt32()) {
        result.value = value->Int32Value(context).ToChecked();

        return result;
    }
    
    if (value->IsNumber()) {
        double num = value->NumberValue(context).ToChecked();
        result.value = Samp_SDK::amx::AMX_FTOC(static_cast<float>(num));

        return result;
    }
    
    if (value->IsBoolean()) {
        result.value = value->BooleanValue(isolate);

        return result;
    }
    
    if (value->IsString()) {
        v8::Local<v8::String> v8_str = value.As<v8::String>();

        v8::String::Utf8Value utf8_value(isolate, v8_str);
        std::string utf8_str(*utf8_value ? *utf8_value : "");

        std::string target_str = Encoding_Converter::Instance().UTF8_To_Target(utf8_str);

        int len = static_cast<int>(target_str.length());
        result.memory = std::make_unique<Samp_SDK::Amx_Scoped_Memory>(amx, len + 1);

        if (!result.memory || !result.memory->Is_Valid()) {
            result.value = 0;

            return result;
        }

        cell* phys_addr = result.memory->Get_Phys_Addr();

        for (int i = 0; i < len; i++)
            phys_addr[i] = static_cast<cell>(static_cast<unsigned char>(target_str[i]));

        phys_addr[len] = 0;

        result.value = result.memory->Get_Amx_Addr();

        return result;
    }
    
    if (!value->IsObject())
        return result;
    
    v8::Local<v8::Object> obj = value.As<v8::Object>();
    
    if (Has_Field(isolate, context, obj, string_cache.kainure_ptr_field.Get(isolate))) {
        v8::Local<v8::Value> parent_val;
        v8::MaybeLocal<v8::Value> maybe_parent = obj->Get(context, string_cache.parent_field.Get(isolate));
        
        if (maybe_parent.ToLocal(&parent_val) && parent_val->IsObject()) {
            v8::Local<v8::Object> parent_obj = parent_val.As<v8::Object>();
            v8::Local<v8::Value> val_prop;
            
            if (Fast_Get_Value(isolate, context, parent_obj, val_prop)) {
                if (val_prop->IsNumber()) {
                    double num = val_prop->NumberValue(context).FromMaybe(0.0);
                    bool is_float = (num != std::floor(num));
                    
                    return Create_Ref_Struct(parent_obj, is_float ? Samp_SDK::amx::AMX_FTOC(static_cast<float>(num)) : static_cast<cell>(num), amx, is_float ? Ref_Type::Float : Ref_Type::Int);
                }
                else if (val_prop->IsBoolean())
                    return Create_Ref_Struct(parent_obj, val_prop->BooleanValue(isolate) ? 1 : 0, amx, Ref_Type::Bool);
                else if (val_prop->IsString()) {
                    v8::Local<v8::String> v8_str = val_prop.As<v8::String>();

                    v8::String::Utf8Value utf8_value(isolate, v8_str);
                    std::string utf8_str(*utf8_value ? *utf8_value : "");

                    std::string target_str = Encoding_Converter::Instance().UTF8_To_Target(utf8_str);

                    int len = static_cast<int>(target_str.length());
                    size_t buffer_size = std::max<size_t>(Constants::DEFAULT_STRING_BUFFER_SIZE, len + 1);

                    result.memory = std::make_unique<Samp_SDK::Amx_Scoped_Memory>(amx, buffer_size);

                    if (result.memory && result.memory->Is_Valid()) {
                        cell* phys_addr = result.memory->Get_Phys_Addr();

                        for (int i = 0; i < len; i++)
                            phys_addr[i] = static_cast<cell>(static_cast<unsigned char>(target_str[i]));

                        phys_addr[len] = 0;

                        result.value = result.memory->Get_Amx_Addr();
                        result.update_data.parent = parent_obj;
                        result.update_data.phys_addr = phys_addr;
                        result.update_data.type = Ref_Type::String;
                        result.update_data.size = buffer_size;
                    }

                    return result;
                }

            }
        }
    }
    
    if (Has_Field(isolate, context, obj, string_cache.kainure_ref_field.Get(isolate))) {
        v8::Local<v8::Value> val_prop;

        if (Fast_Get_Value(isolate, context, obj, val_prop))
            return To_Cell(isolate, context, val_prop, amx);
    }
    
    return result;
}

void Type_Converter::Apply_Updates(v8::Isolate* isolate, v8::Local<v8::Context> context, const Ref_Update_Data* updates, size_t count) {
    if (count == 0 || !updates)
        return;

    string_cache.Ensure_Initialized(isolate);
    v8::Local<v8::String> val_field = string_cache.value_field.Get(isolate);

    for (size_t i = 0; i < count; ++i) {
        const auto& data = updates[i];

        if (data.type == Ref_Type::None)
            continue;
        
        switch (data.type) {
            case Ref_Type::Int:
                data.parent->Set(context, val_field, v8::Integer::New(isolate, *data.phys_addr)).Check();

                break;
            case Ref_Type::Float: {
                float f = Samp_SDK::amx::AMX_CTOF(*data.phys_addr);
                data.parent->Set(context, val_field, v8::Number::New(isolate, f)).Check();

                break;
            }
            case Ref_Type::Bool:
                data.parent->Set(context, val_field, v8::Boolean::New(isolate, *data.phys_addr != 0)).Check();

                break;
            case Ref_Type::String: {
                std::string target_str(data.size, '\0');
                Samp_SDK::amx::Get_String(&target_str[0], data.phys_addr, data.size);

                size_t null_pos = target_str.find('\0');

                if (null_pos != std::string::npos)
                    target_str.resize(null_pos);

                std::string utf8_str = Encoding_Converter::Instance().Target_To_UTF8(target_str);

                data.parent->Set(context, val_field, v8::String::NewFromUtf8(isolate, utf8_str.c_str(), v8::NewStringType::kNormal).ToLocalChecked()).Check();

                break;
            }
            default:
                break;
        }
    }
}

cell Type_Converter::To_Return_Code(v8::Isolate* isolate, v8::Local<v8::Context> context, v8::Local<v8::Value> js_value) {
    if (js_value.IsEmpty() || js_value->IsUndefined() || js_value->IsNull())
        return PLUGIN_PUBLIC_CONTINUE;
    
    if (js_value->IsBoolean())
        return js_value->BooleanValue(isolate) ? PLUGIN_PUBLIC_CONTINUE : PLUGIN_PUBLIC_STOP;
    
    if (js_value->IsInt32())
        return js_value->Int32Value(context).ToChecked();
    
    return PLUGIN_PUBLIC_CONTINUE;
}
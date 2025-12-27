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
#include <cstdint>
#include <algorithm>
//
#include "samp-sdk/amx_api.hpp"
#include "samp-sdk/public_dispatcher.hpp"
#include "samp-sdk/platform.hpp"
//
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
    
    enum class Ref_Type : uint8_t {
        Int,
        Float,
        Bool
    };
    
    SAMP_SDK_FORCE_INLINE void Update_Int_Ref(v8::Isolate* isolate, v8::Local<v8::Object> parent, v8::Local<v8::Context> ctx, cell* addr) {
        parent->Set(ctx, string_cache.value_field.Get(isolate), v8::Integer::New(isolate, *addr)).Check();
    }
    
    SAMP_SDK_FORCE_INLINE void Update_Float_Ref(v8::Isolate* isolate, v8::Local<v8::Object> parent, v8::Local<v8::Context> ctx, cell* addr) {
        float f = Samp_SDK::amx::AMX_CTOF(*addr);
        parent->Set(ctx, string_cache.value_field.Get(isolate), v8::Number::New(isolate, f)).Check();
    }
    
    SAMP_SDK_FORCE_INLINE void Update_Bool_Ref(v8::Isolate* isolate, v8::Local<v8::Object> parent, v8::Local<v8::Context> ctx, cell* addr) {
        parent->Set(ctx, string_cache.value_field.Get(isolate), v8::Boolean::New(isolate, *addr != 0)).Check();
    }
    
    Type_Converter::Conversion_Result Create_Ref(v8::Isolate* isolate, v8::Local<v8::Context> context, v8::Local<v8::Object> parent_obj, cell value, AMX* amx, Ref_Type type) {
        Type_Converter::Conversion_Result result;
        result.memory = std::make_unique<Samp_SDK::Amx_Scoped_Memory>(amx, 1);
        
        if (!result.memory || !result.memory->Is_Valid()) {
            result.value = 0;

            return result;
        }
        
        cell* phys_addr = result.memory->Get_Phys_Addr();
        *phys_addr = value;
        result.value = result.memory->Get_Amx_Addr();
        
        switch (type) {
            case Ref_Type::Int:
                result.updater = [isolate, parent_obj, context, phys_addr]() {
                    v8::HandleScope hs(isolate);
                    Update_Int_Ref(isolate, parent_obj, context, phys_addr);
                };

                break;
            case Ref_Type::Float:
                result.updater = [isolate, parent_obj, context, phys_addr]() {
                    v8::HandleScope hs(isolate);
                    Update_Float_Ref(isolate, parent_obj, context, phys_addr);
                };

                break;
            case Ref_Type::Bool:
                result.updater = [isolate, parent_obj, context, phys_addr]() {
                    v8::HandleScope hs(isolate);
                    Update_Bool_Ref(isolate, parent_obj, context, phys_addr);
                };

                break;
        }
        
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
        v8::String::Utf8Value str(isolate, value);
        
        if (!*str) {
            result.value = 0;

            return result;
        }
        
        size_t len = str.length();
        result.memory = std::make_unique<Samp_SDK::Amx_Scoped_Memory>(amx, len + 1);
        
        if (!result.memory || !result.memory->Is_Valid()) {
            result.value = 0;

            return result;
        }
        
        Samp_SDK::amx::Set_String(result.memory->Get_Phys_Addr(), *str, len + 1);
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
                    
                    if (is_float)
                        return Create_Ref(isolate, context, parent_obj, Samp_SDK::amx::AMX_FTOC(static_cast<float>(num)), amx, Ref_Type::Float);
                    else
                        return Create_Ref(isolate, context, parent_obj, static_cast<cell>(num), amx, Ref_Type::Int);
                }
                else if (val_prop->IsBoolean())
                    return Create_Ref(isolate, context, parent_obj, val_prop->BooleanValue(isolate) ? 1 : 0, amx, Ref_Type::Bool);
                else if (val_prop->IsString()) {
                    v8::String::Utf8Value str(isolate, val_prop);
                    size_t len = str.length();
                    size_t buffer_size = std::max<size_t>(Constants::DEFAULT_STRING_BUFFER_SIZE, len + 1);
                    
                    result.memory = std::make_unique<Samp_SDK::Amx_Scoped_Memory>(amx, buffer_size);

                    if (result.memory && result.memory->Is_Valid()) {
                        Samp_SDK::amx::Set_String(result.memory->Get_Phys_Addr(), *str, buffer_size);
                        result.value = result.memory->Get_Amx_Addr();
                        
                        cell* phys_addr = result.memory->Get_Phys_Addr();

                        result.updater = [isolate, parent_obj, context, phys_addr, buffer_size]() {
                            v8::HandleScope hs(isolate);
                            std::string result_str(buffer_size, '\0');

                            Samp_SDK::amx::Get_String(&result_str[0], phys_addr, buffer_size);
                            
                            size_t null_pos = result_str.find('\0');

                            if (null_pos != std::string::npos)
                                result_str.resize(null_pos);
                            
                            parent_obj->Set(context, string_cache.value_field.Get(isolate), v8::String::NewFromUtf8(isolate, result_str.c_str()).ToLocalChecked()).Check();
                        };
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

cell Type_Converter::To_Return_Code(v8::Isolate* isolate, v8::Local<v8::Context> context, v8::Local<v8::Value> js_value) {
    if (js_value.IsEmpty() || js_value->IsUndefined() || js_value->IsNull())
        return PLUGIN_PUBLIC_CONTINUE;
    
    if (js_value->IsBoolean())
        return js_value->BooleanValue(isolate) ? PLUGIN_PUBLIC_CONTINUE : PLUGIN_PUBLIC_STOP;
    
    if (js_value->IsNumber())
        return js_value->Int32Value(context).ToChecked();
    
    return PLUGIN_PUBLIC_CONTINUE;
}
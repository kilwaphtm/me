#include "value_inspector.h"
#include <android/log.h>
#include <cstring>
#include <string>
struct Il2CppStringLayout
{
    void* klass;
    void* monitor;
    int32_t length;
    uint16_t chars[1];
};

#define LOGI(...) \
    __android_log_print(ANDROID_LOG_INFO, "MY_CUSTOM_SO", __VA_ARGS__)
// =========================================================
// TYPE NAME
// =========================================================

const char* Il2CppValueInspector::GetTypeName(const Il2CppType* type)
{
    if (!type || !g_il2cpp_type_get_name)
        return "<null>";

    const char* name = g_il2cpp_type_get_name(type);

    return name ? name : "<unknown>";
}

// =========================================================
// TYPE KIND
// =========================================================

const char* Il2CppValueInspector::GetTypeKindName(const Il2CppType* type)
{
    if (!type || !g_il2cpp_type_get_type)
        return "Unknown";

    switch (g_il2cpp_type_get_type(type))
    {
        case IL2CPP_TYPE_VOID:        return "Void";
        case IL2CPP_TYPE_BOOLEAN:     return "Boolean";
        case IL2CPP_TYPE_CHAR:        return "Char";
        case IL2CPP_TYPE_I1:          return "Int8";
        case IL2CPP_TYPE_U1:          return "UInt8";
        case IL2CPP_TYPE_I2:          return "Int16";
        case IL2CPP_TYPE_U2:          return "UInt16";
        case IL2CPP_TYPE_I4:          return "Int32";
        case IL2CPP_TYPE_U4:          return "UInt32";
        case IL2CPP_TYPE_I8:          return "Int64";
        case IL2CPP_TYPE_U8:          return "UInt64";
        case IL2CPP_TYPE_R4:          return "Float";
        case IL2CPP_TYPE_R8:          return "Double";
        case IL2CPP_TYPE_STRING:      return "String";
        case IL2CPP_TYPE_PTR:         return "Pointer";
        case IL2CPP_TYPE_BYREF:       return "ByRef";
        case IL2CPP_TYPE_VALUETYPE:   return "ValueType";
        case IL2CPP_TYPE_CLASS:       return "Class";
        case IL2CPP_TYPE_VAR:         return "GenericParameter";
        case IL2CPP_TYPE_ARRAY:       return "Array";
        case IL2CPP_TYPE_GENERICINST: return "GenericInstance";
        case IL2CPP_TYPE_TYPEDBYREF:  return "TypedReference";
        case IL2CPP_TYPE_I:           return "IntPtr";
        case IL2CPP_TYPE_U:           return "UIntPtr";
        case IL2CPP_TYPE_FNPTR:       return "FunctionPointer";
        case IL2CPP_TYPE_OBJECT:      return "Object";
        case IL2CPP_TYPE_SZARRAY:     return "SZArray";
        case IL2CPP_TYPE_MVAR:        return "MethodGenericParameter";
        case IL2CPP_TYPE_CMOD_REQD:   return "RequiredModifier";
        case IL2CPP_TYPE_CMOD_OPT:    return "OptionalModifier";
        case IL2CPP_TYPE_INTERNAL:    return "Internal";
        case IL2CPP_TYPE_MODIFIER:    return "Modifier";
        case IL2CPP_TYPE_SENTINEL:    return "Sentinel";
        case IL2CPP_TYPE_PINNED:      return "Pinned";
        case IL2CPP_TYPE_ENUM:        return "Enum";

        default:
            return "Unknown";
    }
}

// =========================================================
// CLASS INSPECTION
// =========================================================

bool Il2CppValueInspector::InspectClass(const Il2CppType* type)
{
    if (!type)
    {
        LOGI("[INSPECTOR] Type is null");
        return false;
    }

    if (!g_il2cpp_type_get_class_or_element_class)
    {
        LOGI("[INSPECTOR] il2cpp_type_get_class_or_element_class unavailable");
        return false;
    }

    Il2CppClass* klass =
            g_il2cpp_type_get_class_or_element_class(type);

    if (!klass)
    {
        LOGI("[INSPECTOR] Failed to resolve Il2CppClass");
        return false;
    }

    const char* className =
            g_il2cpp_class_get_name
            ? g_il2cpp_class_get_name(klass)
            : "<unknown>";

    const char* namespaceName =
            g_il2cpp_class_get_namespace
            ? g_il2cpp_class_get_namespace(klass)
            : "<unknown>";

    LOGI(
            "[INSPECTOR] Class: %s.%s",
            namespaceName ? namespaceName : "<null>",
            className ? className : "<null>"
    );

    bool isEnum =
            g_il2cpp_class_is_enum
            ? g_il2cpp_class_is_enum(klass)
            : false;

    LOGI(
            "[INSPECTOR] IsEnum: %s",
            isEnum ? "true" : "false"
    );


    if (g_il2cpp_class_value_size)
    {
        uint32_t alignment = 0;

        int32_t size =
                g_il2cpp_class_value_size(
                        klass,
                        &alignment
                );

        LOGI(
                "[INSPECTOR] ValueSize: %d | Alignment: %u",
                size,
                alignment
        );
    }

    if (!g_il2cpp_class_num_fields ||
        !g_il2cpp_class_get_fields)
    {
        LOGI("[INSPECTOR] Field APIs unavailable");
        return true;
    }

    size_t fieldCount =
            g_il2cpp_class_num_fields(klass);

    LOGI(
            "[INSPECTOR] Field Count: %zu",
            fieldCount
    );

    void* iterator = nullptr;

    size_t index = 0;

    while (true)
    {
        FieldInfo* field =
                g_il2cpp_class_get_fields(
                        klass,
                        &iterator
                );

        if (!field)
            break;

        const char* fieldName =
                g_il2cpp_field_get_name
                ? g_il2cpp_field_get_name(field)
                : "<unknown>";

        const Il2CppType* fieldType =
                g_il2cpp_field_get_type
                ? g_il2cpp_field_get_type(field)
                : nullptr;

        size_t offset =
                g_il2cpp_field_get_offset
                ? g_il2cpp_field_get_offset(field)
                : 0;

        LOGI(
                "[INSPECTOR] Field[%zu]: name=%s type=%s kind=%s offset=%zu",
                index,
                fieldName ? fieldName : "<null>",
                GetTypeName(fieldType),
                GetTypeKindName(fieldType),
                offset
        );

        ++index;
    }

    return true;
}

void Il2CppValueInspector::InspectEnumValue(
        const Il2CppType* type,
        const void* data)
{
    if (!type || !data)
    {
        LOGI("[INSPECTOR] Enum type or data is null");
        return;
    }

    Il2CppClass* klass =
            g_il2cpp_type_get_class_or_element_class
            ? g_il2cpp_type_get_class_or_element_class(type)
            : nullptr;

    if (!klass)
    {
        LOGI("[INSPECTOR] Failed to resolve enum class");
        return;
    }

    const char* className =
            g_il2cpp_class_get_name
            ? g_il2cpp_class_get_name(klass)
            : "<unknown>";

    int32_t value = 0;

    std::memcpy(
            &value,
            data,
            sizeof(value)
    );

    LOGI(
            "[INSPECTOR] Enum: %s | Value: %d",
            className ? className : "<null>",
            value
    );
}



struct Il2CppGenericInst
{
    uint32_t type_argc;
    const Il2CppType** type_argv;
};

struct Il2CppGenericContext
{
    const Il2CppGenericInst* class_inst;
    const Il2CppGenericInst* method_inst;
};

struct Il2CppGenericClass
{
    Il2CppClass* typeDefinition;
    Il2CppGenericContext context;
};

InspectedValue Il2CppValueInspector::InspectValue(
        const Il2CppType* type,
        const void* data)
{
    InspectedValue result;

    result.type = type;

    if (!type || !data)
    {
        result.isNull = true;
        result.displayValue = "<null>";

        LOGI("[INSPECTOR] Value or type is null");

        return result;
    }

    result.typeKind =
            g_il2cpp_type_get_type
            ? g_il2cpp_type_get_type(type)
            : -1;

    result.typeName = GetTypeName(type);

    switch (result.typeKind)
    {
        case IL2CPP_TYPE_BOOLEAN:
        {
            bool value =
                    *reinterpret_cast<const bool*>(data);

            result.boolValue = value;
            result.displayValue =
                    value ? "true" : "false";

            LOGI(
                    "[INSPECTOR] Value: Boolean = %s",
                    value ? "true" : "false"
            );

            break;
        }

        case IL2CPP_TYPE_CHAR:
        {
            uint16_t value =
                    *reinterpret_cast<const uint16_t*>(data);

            result.unsignedValue = value;
            result.displayValue =
                    std::to_string(value);

            LOGI(
                    "[INSPECTOR] Value: Char = %u",
                    value
            );

            break;
        }

        case IL2CPP_TYPE_I1:
        {
            int8_t value =
                    *reinterpret_cast<const int8_t*>(data);

            result.signedValue = value;
            result.displayValue =
                    std::to_string(
                            static_cast<int>(value)
                    );

            LOGI(
                    "[INSPECTOR] Value: Int8 = %d",
                    static_cast<int>(value)
            );

            break;
        }

        case IL2CPP_TYPE_U1:
        {
            uint8_t value =
                    *reinterpret_cast<const uint8_t*>(data);

            result.unsignedValue = value;
            result.displayValue =
                    std::to_string(
                            static_cast<unsigned>(value)
                    );

            LOGI(
                    "[INSPECTOR] Value: UInt8 = %u",
                    static_cast<unsigned>(value)
            );

            break;
        }

        case IL2CPP_TYPE_I2:
        {
            int16_t value =
                    *reinterpret_cast<const int16_t*>(data);

            result.signedValue = value;
            result.displayValue =
                    std::to_string(
                            static_cast<int>(value)
                    );

            LOGI(
                    "[INSPECTOR] Value: Int16 = %d",
                    static_cast<int>(value)
            );

            break;
        }

        case IL2CPP_TYPE_U2:
        {
            uint16_t value =
                    *reinterpret_cast<const uint16_t*>(data);

            result.unsignedValue = value;
            result.displayValue =
                    std::to_string(
                            static_cast<unsigned>(value)
                    );

            LOGI(
                    "[INSPECTOR] Value: UInt16 = %u",
                    static_cast<unsigned>(value)
            );

            break;
        }

        case IL2CPP_TYPE_I4:
        {
            int32_t value =
                    *reinterpret_cast<const int32_t*>(data);

            result.signedValue = value;
            result.displayValue =
                    std::to_string(value);

            LOGI(
                    "[INSPECTOR] Value: Int32 = %d",
                    value
            );

            break;
        }

        case IL2CPP_TYPE_U4:
        {
            uint32_t value =
                    *reinterpret_cast<const uint32_t*>(data);

            result.unsignedValue = value;
            result.displayValue =
                    std::to_string(value);

            LOGI(
                    "[INSPECTOR] Value: UInt32 = %u",
                    value
            );

            break;
        }

        case IL2CPP_TYPE_I8:
        {
            int64_t value =
                    *reinterpret_cast<const int64_t*>(data);

            result.signedValue = value;
            result.displayValue =
                    std::to_string(value);

            LOGI(
                    "[INSPECTOR] Value: Int64 = %lld",
                    static_cast<long long>(value)
            );

            break;
        }

        case IL2CPP_TYPE_U8:
        {
            uint64_t value =
                    *reinterpret_cast<const uint64_t*>(data);

            result.unsignedValue = value;
            result.displayValue =
                    std::to_string(value);

            LOGI(
                    "[INSPECTOR] Value: UInt64 = %llu",
                    static_cast<unsigned long long>(value)
            );

            break;
        }

        case IL2CPP_TYPE_R4:
        {
            float value =
                    *reinterpret_cast<const float*>(data);

            result.floatValue = value;
            result.displayValue =
                    std::to_string(value);

            LOGI(
                    "[INSPECTOR] Value: Float = %f",
                    value
            );

            break;
        }

        case IL2CPP_TYPE_R8:
        {
            double value =
                    *reinterpret_cast<const double*>(data);

            result.floatValue = value;
            result.displayValue =
                    std::to_string(value);

            LOGI(
                    "[INSPECTOR] Value: Double = %f",
                    value
            );

            break;
        }

        case IL2CPP_TYPE_STRING:
        {
            void* stringObject =
                    *reinterpret_cast<void* const*>(data);

            result.pointerValue = stringObject;

            if (!stringObject)
            {
                result.isNull = true;
                result.displayValue = "<null>";

                LOGI(
                        "[INSPECTOR] String = <null>"
                );

                break;
            }

            Il2CppStringLayout* str =
                    reinterpret_cast<Il2CppStringLayout*>(
                            stringObject
                    );

            if (str->length <= 0)
            {
                result.stringValue = "";
                result.displayValue = "";

                LOGI(
                        "[INSPECTOR] String = \"\""
                );

                break;
            }

            std::string stringValue;

            stringValue.reserve(
                    static_cast<size_t>(str->length)
            );

            for (int32_t i = 0; i < str->length; ++i)
            {
                uint16_t c = str->chars[i];

                if (c < 0x80)
                {
                    stringValue.push_back(
                            static_cast<char>(c)
                    );
                }
                else if (c < 0x800)
                {
                    stringValue.push_back(
                            static_cast<char>(
                                    0xC0 | (c >> 6)
                            )
                    );

                    stringValue.push_back(
                            static_cast<char>(
                                    0x80 | (c & 0x3F)
                            )
                    );
                }
                else
                {
                    stringValue.push_back(
                            static_cast<char>(
                                    0xE0 | (c >> 12)
                            )
                    );

                    stringValue.push_back(
                            static_cast<char>(
                                    0x80 |
                                    ((c >> 6) & 0x3F)
                            )
                    );

                    stringValue.push_back(
                            static_cast<char>(
                                    0x80 | (c & 0x3F)
                            )
                    );
                }
            }

            result.stringValue = stringValue;
            result.displayValue = stringValue;

            LOGI(
                    "[INSPECTOR] String = \"%s\"",
                    stringValue.c_str()
            );

            break;
        }

        case IL2CPP_TYPE_VALUETYPE:
        {
            Il2CppClass* klass =
                    g_il2cpp_type_get_class_or_element_class
                    ? g_il2cpp_type_get_class_or_element_class(type)
                    : nullptr;

            result.klass = klass;

            if (klass &&
                g_il2cpp_class_is_enum &&
                g_il2cpp_class_is_enum(klass))
            {
                result.isEnum = true;

                int32_t value =
                        *reinterpret_cast<const int32_t*>(data);

                result.signedValue = value;
                result.displayValue =
                        std::to_string(value);

                LOGI(
                        "[INSPECTOR] Enum: %s | Value: %d",
                        result.typeName.c_str(),
                        value
                );
            }
            else
            {
                if (!g_il2cpp_class_get_fields ||
                    !g_il2cpp_field_get_name ||
                    !g_il2cpp_field_get_type ||
                    !g_il2cpp_field_get_offset ||
                    !g_il2cpp_field_get_flags)
                {
                    LOGI(
                            "[INSPECTOR] ValueType field APIs unavailable: %s",
                            result.typeName.c_str()
                    );

                    break;
                }

                LOGI(
                        "[INSPECTOR] ValueType Fields: %s",
                        result.typeName.c_str()
                );

                void* iterator = nullptr;

                size_t index = 0;

                while (true)
                {
                    FieldInfo* field =
                            g_il2cpp_class_get_fields(
                                    klass,
                                    &iterator
                            );

                    if (!field)
                        break;

                    int32_t flags =
                            g_il2cpp_field_get_flags(field);

                    // FIELD_ATTRIBUTE_STATIC = 0x0010
                    if ((flags & 0x0010) != 0)
                    {
                        ++index;
                        continue;
                    }

                    const char* fieldName =
                            g_il2cpp_field_get_name(field);

                    const Il2CppType* fieldType =
                            g_il2cpp_field_get_type(field);

                    size_t offset =
                            g_il2cpp_field_get_offset(field);

                    LOGI(
                            "[INSPECTOR] Field[%zu]: %s | type=%s | kind=%s | offset=%zu",
                            index,
                            fieldName ? fieldName : "<unknown>",
                            GetTypeName(fieldType),
                            GetTypeKindName(fieldType),
                            offset
                    );

                    const uint8_t* fieldData =
                            reinterpret_cast<const uint8_t*>(data) + offset;

                    InspectedValue fieldValue =
                            InspectValue(
                                    fieldType,
                                    fieldData
                            );

                    LOGI(
                            "[INSPECTOR] Field[%zu] Value: %s = %s",
                            index,
                            fieldName ? fieldName : "<unknown>",
                            fieldValue.displayValue.c_str()
                    );

                    ++index;
                }
            }

            break;
        }

        case IL2CPP_TYPE_ENUM:
        {
            result.isEnum = true;

            int32_t value =
                    *reinterpret_cast<const int32_t*>(data);

            result.signedValue = value;
            result.displayValue =
                    std::to_string(value);

            LOGI(
                    "[INSPECTOR] Enum: %s | Value: %d",
                    result.typeName.c_str(),
                    value
            );

            break;
        }
        case IL2CPP_TYPE_GENERICINST:
        {
            result.klass =
                    g_il2cpp_type_get_class_or_element_class
                    ? g_il2cpp_type_get_class_or_element_class(type)
                    : nullptr;

            LOGI(
                    "[INSPECTOR] GenericInstance: %s",
                    result.typeName.c_str()
            );

            if (!type->data)
            {
                LOGI("[INSPECTOR] GenericInstance data = NULL");
                break;
            }

            auto* genericClass =
                    reinterpret_cast<const Il2CppGenericClass*>(type->data);

            if (!genericClass)
            {
                LOGI("[INSPECTOR] GenericClass = NULL");
                break;
            }

            const Il2CppGenericInst* inst =
                    genericClass->context.class_inst;

            if (!inst)
            {
                LOGI("[INSPECTOR] Generic class_inst = NULL");
                break;
            }

            LOGI(
                    "[INSPECTOR] Generic Args Count: %u",
                    inst->type_argc
            );

            for (uint32_t i = 0; i < inst->type_argc; ++i)
            {
                const Il2CppType* argType =
                        inst->type_argv[i];

                if (!argType)
                {
                    LOGI(
                            "[INSPECTOR] Generic Arg[%u]: NULL",
                            i
                    );
                    continue;
                }

                const char* argName =
                        g_il2cpp_type_get_name
                        ? g_il2cpp_type_get_name(argType)
                        : "<unknown>";

                int argKind =
                        g_il2cpp_type_get_type
                        ? g_il2cpp_type_get_type(argType)
                        : -1;

                LOGI(
                        "[INSPECTOR] Generic Arg[%u]: %s | kind=%d",
                        i,
                        argName ? argName : "<null>",
                        argKind
                );
            }

            break;
        }
        default:
        {
            LOGI(
                    "[INSPECTOR] Value inspection not implemented for kind=%s type=%s",
                    GetTypeKindName(type),
                    result.typeName.c_str()
            );

            break;
        }
    }

    return result;
}
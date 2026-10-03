#pragma once

#include <cstddef>
#include <cstdint>

// =========================================================
// IL2CPP TYPES
// =========================================================

struct Il2CppDomain;
struct Il2CppAssembly;
struct Il2CppImage;
struct Il2CppClass;
struct Il2CppObject
{
    Il2CppClass* klass;
    void* monitor;
};
using Il2CppMethodPointer = void(*)();


struct Il2CppType
{
    const void* data;
    uint32_t bits;
};
struct Il2CppException;
struct Il2CppThread;
struct MethodInfo
{
    void* methodPointer;
};
struct FieldInfo;
struct PropertyInfo;
struct Il2CppDelegate
{
    Il2CppObject object;

    Il2CppMethodPointer method_ptr;
    Il2CppMethodPointer invoke_impl;

    Il2CppObject* target;
    const MethodInfo* method;

    void* delegate_trampoline;
    intptr_t extraArg;

    Il2CppObject* invoke_impl_this;

    void* interp_method;
    void* interp_invoke_impl;

    void* method_info;
    void* original_method_info;

    Il2CppObject* data;

    bool method_is_virtual;
};
// =========================================================
// IL2CPP TYPE ENUM
// =========================================================

enum Il2CppTypeEnum
{
    IL2CPP_TYPE_END        = 0x00,
    IL2CPP_TYPE_VOID       = 0x01,
    IL2CPP_TYPE_BOOLEAN    = 0x02,
    IL2CPP_TYPE_CHAR       = 0x03,
    IL2CPP_TYPE_I1         = 0x04,
    IL2CPP_TYPE_U1         = 0x05,
    IL2CPP_TYPE_I2         = 0x06,
    IL2CPP_TYPE_U2         = 0x07,
    IL2CPP_TYPE_I4         = 0x08,
    IL2CPP_TYPE_U4         = 0x09,
    IL2CPP_TYPE_I8         = 0x0A,
    IL2CPP_TYPE_U8         = 0x0B,
    IL2CPP_TYPE_R4         = 0x0C,
    IL2CPP_TYPE_R8         = 0x0D,
    IL2CPP_TYPE_STRING     = 0x0E,
    IL2CPP_TYPE_PTR        = 0x0F,
    IL2CPP_TYPE_BYREF      = 0x10,
    IL2CPP_TYPE_VALUETYPE  = 0x11,
    IL2CPP_TYPE_CLASS      = 0x12,
    IL2CPP_TYPE_VAR        = 0x13,
    IL2CPP_TYPE_ARRAY      = 0x14,
    IL2CPP_TYPE_GENERICINST= 0x15,
    IL2CPP_TYPE_TYPEDBYREF = 0x16,
    IL2CPP_TYPE_I          = 0x18,
    IL2CPP_TYPE_U          = 0x19,
    IL2CPP_TYPE_FNPTR      = 0x1B,
    IL2CPP_TYPE_OBJECT     = 0x1C,
    IL2CPP_TYPE_SZARRAY    = 0x1D,
    IL2CPP_TYPE_MVAR       = 0x1E,
    IL2CPP_TYPE_CMOD_REQD  = 0x1F,
    IL2CPP_TYPE_CMOD_OPT   = 0x20,
    IL2CPP_TYPE_INTERNAL   = 0x21,
    IL2CPP_TYPE_MODIFIER   = 0x40,
    IL2CPP_TYPE_SENTINEL   = 0x41,
    IL2CPP_TYPE_PINNED     = 0x45,
    IL2CPP_TYPE_ENUM       = 0x55
};
// =========================================================
// IL2CPP METHOD API
// =========================================================

using il2cpp_class_get_methods_t =
        const MethodInfo* (*)(Il2CppClass*, void**);

using il2cpp_class_get_method_from_name_t =
        const MethodInfo* (*)(Il2CppClass*, const char*, int);

using il2cpp_method_get_name_t =
        const char* (*)(const MethodInfo*);

using il2cpp_method_get_class_t =
        Il2CppClass* (*)(const MethodInfo*);

using il2cpp_method_get_declaring_type_t =
        Il2CppClass* (*)(const MethodInfo*);

using il2cpp_method_get_flags_t =
        uint32_t (*)(const MethodInfo*, uint32_t*);

using il2cpp_method_get_token_t =
        uint32_t (*)(const MethodInfo*);

using il2cpp_method_get_param_t =
        const Il2CppType* (*)(const MethodInfo*, uint32_t);

using il2cpp_method_get_param_count_t =
        uint32_t (*)(const MethodInfo*);

using il2cpp_method_get_param_name_t =
        const char* (*)(const MethodInfo*, uint32_t);

using il2cpp_method_get_return_type_t =
        const Il2CppType* (*)(const MethodInfo*);

using il2cpp_method_is_generic_t =
        bool (*)(const MethodInfo*);

using il2cpp_method_is_inflated_t =
        bool (*)(const MethodInfo*);

using il2cpp_method_is_instance_t =
        bool (*)(const MethodInfo*);


// =========================================================
// IL2CPP TYPE API
// =========================================================

using il2cpp_type_get_name_t =
        const char* (*)(const Il2CppType*);

using il2cpp_type_get_name_chunked_t =
        void (*)(const Il2CppType*, void*);

using il2cpp_type_get_type_t =
        int (*)(const Il2CppType*);

using il2cpp_type_get_class_or_element_class_t =
        Il2CppClass* (*)(const Il2CppType*);

using il2cpp_type_get_object_t =
        Il2CppObject* (*)(const Il2CppType*);

using il2cpp_type_get_reflection_name_t =
        const char* (*)(const Il2CppType*);

using il2cpp_type_get_assembly_qualified_name_t =
        const char* (*)(const Il2CppType*);

using il2cpp_type_is_pointer_type_t =
        bool (*)(const Il2CppType*);

using il2cpp_type_is_byref_t =
        bool (*)(const Il2CppType*);

using il2cpp_type_is_static_t =
        bool (*)(const Il2CppType*);

using il2cpp_type_equals_t =
        bool (*)(const Il2CppType*, const Il2CppType*);


// =========================================================
// IL2CPP CLASS API
// =========================================================

using il2cpp_class_from_name_t =
        Il2CppClass* (*)(const Il2CppImage*, const char*, const char*);

using il2cpp_class_from_type_t =
        Il2CppClass* (*)(const Il2CppType*);

using il2cpp_class_get_name_t =
        const char* (*)(const Il2CppClass*);

using il2cpp_class_get_namespace_t =
        const char* (*)(const Il2CppClass*);

using il2cpp_class_get_image_t =
        Il2CppImage* (*)(const Il2CppClass*);

using il2cpp_class_get_parent_t =
        Il2CppClass* (*)(Il2CppClass*);

using il2cpp_class_get_type_t =
        const Il2CppType* (*)(Il2CppClass*);

using il2cpp_class_get_element_class_t =
        Il2CppClass* (*)(Il2CppClass*);

using il2cpp_class_get_assemblyname_t =
        const char* (*)(const Il2CppClass*);

using il2cpp_class_is_valuetype_t =
        bool (*)(Il2CppClass*);

using il2cpp_class_is_enum_t =
        bool (*)(Il2CppClass*);

using il2cpp_class_is_interface_t =
        bool (*)(Il2CppClass*);

using il2cpp_class_is_abstract_t =
        bool (*)(Il2CppClass*);

using il2cpp_class_is_generic_t =
        bool (*)(Il2CppClass*);

using il2cpp_class_is_inflated_t =
        bool (*)(Il2CppClass*);

using il2cpp_class_is_blittable_t =
        bool (*)(Il2CppClass*);

using il2cpp_class_value_size_t =
        int32_t (*)(Il2CppClass*, uint32_t*);

using il2cpp_class_instance_size_t =
        size_t (*)(Il2CppClass*);

using il2cpp_class_num_fields_t =
        size_t (*)(const Il2CppClass*);

using il2cpp_class_has_parent_t =
        bool (*)(Il2CppClass*, Il2CppClass*);

using il2cpp_class_is_assignable_from_t =
        bool (*)(Il2CppClass*, Il2CppClass*);


// =========================================================
// IL2CPP FIELD API
// =========================================================

using il2cpp_class_get_fields_t =
        FieldInfo* (*)(Il2CppClass*, void**);

using il2cpp_class_get_field_from_name_t =
        FieldInfo* (*)(Il2CppClass*, const char*);

using il2cpp_field_get_name_t =
        const char* (*)(FieldInfo*);

using il2cpp_field_get_type_t =
        const Il2CppType* (*)(FieldInfo*);

using il2cpp_field_get_offset_t =
        size_t (*)(FieldInfo*);

using il2cpp_field_get_parent_t =
        Il2CppClass* (*)(FieldInfo*);

using il2cpp_field_get_flags_t =
        int32_t (*)(FieldInfo*);

using il2cpp_field_get_value_t =
        void (*)(Il2CppObject*, FieldInfo*, void*);

using il2cpp_field_get_value_object_t =
        Il2CppObject* (*)(FieldInfo*, Il2CppObject*);

using il2cpp_field_set_value_t =
        void (*)(Il2CppObject*, FieldInfo*, void*);

using il2cpp_field_set_value_object_t =
        void (*)(Il2CppObject*, FieldInfo*, Il2CppObject*);

using il2cpp_field_static_get_value_t =
        void (*)(FieldInfo*, void*);

using il2cpp_field_static_set_value_t =
        void (*)(FieldInfo*, void*);

using il2cpp_field_is_literal_t =
        bool (*)(FieldInfo*);


// =========================================================
// IL2CPP PROPERTY API
// =========================================================

using il2cpp_class_get_properties_t =
        PropertyInfo* (*)(Il2CppClass*, void**);

using il2cpp_class_get_property_from_name_t =
        PropertyInfo* (*)(Il2CppClass*, const char*);

using il2cpp_property_get_name_t =
        const char* (*)(PropertyInfo*);

using il2cpp_property_get_parent_t =
        Il2CppClass* (*)(PropertyInfo*);

using il2cpp_property_get_flags_t =
        uint32_t (*)(PropertyInfo*);

using il2cpp_property_get_get_method_t =
        const MethodInfo* (*)(PropertyInfo*);

using il2cpp_property_get_set_method_t =
        const MethodInfo* (*)(PropertyInfo*);


// =========================================================
// IL2CPP OBJECT API
// =========================================================

using il2cpp_object_get_class_t =
        Il2CppClass* (*)(Il2CppObject*);

using il2cpp_object_get_size_t =
        size_t (*)(Il2CppObject*);

using il2cpp_object_get_virtual_method_t =
        const MethodInfo* (*)(Il2CppObject*, const MethodInfo*);

using il2cpp_object_unbox_t =
        void* (*)(Il2CppObject*);

using il2cpp_object_new_t =
        Il2CppObject* (*)(Il2CppClass*);


// =========================================================
// IL2CPP RUNTIME API
// =========================================================

using il2cpp_runtime_invoke_t =
        Il2CppObject* (*)
                (
                        const MethodInfo*,
                        void*,
                        void**,
                        Il2CppObject**
                );

using il2cpp_runtime_invoke_convert_args_t =
        Il2CppObject* (*)
                (
                        const MethodInfo*,
                        void*,
                        Il2CppObject**,
                        int,
                        Il2CppObject**
                );

using il2cpp_runtime_class_init_t =
        void (*)(Il2CppClass*);

using il2cpp_runtime_object_init_t =
        void (*)(Il2CppObject*);

using il2cpp_runtime_object_init_exception_t =
        void (*)(Il2CppObject*, Il2CppException**);

using il2cpp_value_box_t =
        Il2CppObject* (*)(Il2CppClass*, void*);


// =========================================================
// IL2CPP IMAGE / ASSEMBLY API
// =========================================================

using il2cpp_domain_get_t =
        Il2CppDomain* (*)();
using il2cpp_thread_attach_t =
        Il2CppThread* (*)(Il2CppDomain*);

using il2cpp_thread_get_all_attached_threads_t =
        Il2CppThread** (*)(size_t* size);

using il2cpp_thread_current_t =
        Il2CppThread* (*)();

extern il2cpp_thread_get_all_attached_threads_t
        g_il2cpp_thread_get_all_attached_threads;

extern il2cpp_thread_current_t
        g_il2cpp_thread_current;

using il2cpp_domain_get_assemblies_t =
        Il2CppAssembly** (*)(Il2CppDomain*, size_t*);

using il2cpp_assembly_get_image_t =
        Il2CppImage* (*)(const Il2CppAssembly*);

using il2cpp_image_get_name_t =
        const char* (*)(const Il2CppImage*);

using il2cpp_image_get_filename_t =
        const char* (*)(const Il2CppImage*);

using il2cpp_image_get_class_count_t =
        size_t (*)(const Il2CppImage*);

using il2cpp_image_get_class_t =
        Il2CppClass* (*)(const Il2CppImage*, size_t);

// =========================================================
// GLOBAL IL2CPP HANDLE
// =========================================================

extern void* g_Il2CppHandle;


// =========================================================
// GLOBAL IL2CPP API
// =========================================================

// Domain / Assembly / Image

extern il2cpp_domain_get_t
        g_il2cpp_domain_get;
extern il2cpp_thread_attach_t
        g_il2cpp_thread_attach;

extern il2cpp_domain_get_assemblies_t
        g_il2cpp_domain_get_assemblies;

extern il2cpp_assembly_get_image_t
        g_il2cpp_assembly_get_image;

extern il2cpp_image_get_name_t
        g_il2cpp_image_get_name;

extern il2cpp_image_get_filename_t
        g_il2cpp_image_get_filename;

extern il2cpp_image_get_class_count_t
        g_il2cpp_image_get_class_count;

extern il2cpp_image_get_class_t
        g_il2cpp_image_get_class;


// Class

extern il2cpp_class_from_name_t
        g_il2cpp_class_from_name;

extern il2cpp_class_from_type_t
        g_il2cpp_class_from_type;

extern il2cpp_class_get_name_t
        g_il2cpp_class_get_name;

extern il2cpp_class_get_namespace_t
        g_il2cpp_class_get_namespace;

extern il2cpp_class_get_image_t
        g_il2cpp_class_get_image;

extern il2cpp_class_get_parent_t
        g_il2cpp_class_get_parent;

extern il2cpp_class_get_type_t
        g_il2cpp_class_get_type;

extern il2cpp_class_get_element_class_t
        g_il2cpp_class_get_element_class;

extern il2cpp_class_get_assemblyname_t
        g_il2cpp_class_get_assemblyname;

extern il2cpp_class_is_valuetype_t
        g_il2cpp_class_is_valuetype;

extern il2cpp_class_is_enum_t
        g_il2cpp_class_is_enum;

extern il2cpp_class_is_interface_t
        g_il2cpp_class_is_interface;

extern il2cpp_class_is_abstract_t
        g_il2cpp_class_is_abstract;

extern il2cpp_class_is_generic_t
        g_il2cpp_class_is_generic;

extern il2cpp_class_is_inflated_t
        g_il2cpp_class_is_inflated;

extern il2cpp_class_is_blittable_t
        g_il2cpp_class_is_blittable;

extern il2cpp_class_value_size_t
        g_il2cpp_class_value_size;

extern il2cpp_class_instance_size_t
        g_il2cpp_class_instance_size;

extern il2cpp_class_num_fields_t
        g_il2cpp_class_num_fields;

extern il2cpp_class_has_parent_t
        g_il2cpp_class_has_parent;

extern il2cpp_class_is_assignable_from_t
        g_il2cpp_class_is_assignable_from;


// Methods

extern il2cpp_class_get_methods_t
        g_il2cpp_class_get_methods;

extern il2cpp_class_get_method_from_name_t
        g_il2cpp_class_get_method_from_name;

extern il2cpp_method_get_name_t
        g_il2cpp_method_get_name;

extern il2cpp_method_get_class_t
        g_il2cpp_method_get_class;

extern il2cpp_method_get_declaring_type_t
        g_il2cpp_method_get_declaring_type;

extern il2cpp_method_get_flags_t
        g_il2cpp_method_get_flags;

extern il2cpp_method_get_token_t
        g_il2cpp_method_get_token;

extern il2cpp_method_get_param_t
        g_il2cpp_method_get_param;

extern il2cpp_method_get_param_count_t
        g_il2cpp_method_get_param_count;

extern il2cpp_method_get_param_name_t
        g_il2cpp_method_get_param_name;

extern il2cpp_method_get_return_type_t
        g_il2cpp_method_get_return_type;

extern il2cpp_method_is_generic_t
        g_il2cpp_method_is_generic;

extern il2cpp_method_is_inflated_t
        g_il2cpp_method_is_inflated;

extern il2cpp_method_is_instance_t
        g_il2cpp_method_is_instance;


// Type

extern il2cpp_type_get_name_t
        g_il2cpp_type_get_name;

extern il2cpp_type_get_name_chunked_t
        g_il2cpp_type_get_name_chunked;

extern il2cpp_type_get_type_t
        g_il2cpp_type_get_type;

extern il2cpp_type_get_class_or_element_class_t
        g_il2cpp_type_get_class_or_element_class;

extern il2cpp_type_get_object_t
        g_il2cpp_type_get_object;

extern il2cpp_type_get_reflection_name_t
        g_il2cpp_type_get_reflection_name;

extern il2cpp_type_get_assembly_qualified_name_t
        g_il2cpp_type_get_assembly_qualified_name;

extern il2cpp_type_is_pointer_type_t
        g_il2cpp_type_is_pointer_type;

extern il2cpp_type_is_byref_t
        g_il2cpp_type_is_byref;

extern il2cpp_type_is_static_t
        g_il2cpp_type_is_static;

extern il2cpp_type_equals_t
        g_il2cpp_type_equals;


// Fields

extern il2cpp_class_get_fields_t
        g_il2cpp_class_get_fields;

extern il2cpp_class_get_field_from_name_t
        g_il2cpp_class_get_field_from_name;

extern il2cpp_field_get_name_t
        g_il2cpp_field_get_name;

extern il2cpp_field_get_type_t
        g_il2cpp_field_get_type;

extern il2cpp_field_get_offset_t
        g_il2cpp_field_get_offset;

extern il2cpp_field_get_parent_t
        g_il2cpp_field_get_parent;

extern il2cpp_field_get_flags_t
        g_il2cpp_field_get_flags;

extern il2cpp_field_get_value_t
        g_il2cpp_field_get_value;

extern il2cpp_field_get_value_object_t
        g_il2cpp_field_get_value_object;

extern il2cpp_field_set_value_t
        g_il2cpp_field_set_value;

extern il2cpp_field_set_value_object_t
        g_il2cpp_field_set_value_object;

extern il2cpp_field_static_get_value_t
        g_il2cpp_field_static_get_value;

extern il2cpp_field_static_set_value_t
        g_il2cpp_field_static_set_value;

extern il2cpp_field_is_literal_t
        g_il2cpp_field_is_literal;


// Properties

extern il2cpp_class_get_properties_t
        g_il2cpp_class_get_properties;

extern il2cpp_class_get_property_from_name_t
        g_il2cpp_class_get_property_from_name;

extern il2cpp_property_get_name_t
        g_il2cpp_property_get_name;

extern il2cpp_property_get_parent_t
        g_il2cpp_property_get_parent;

extern il2cpp_property_get_flags_t
        g_il2cpp_property_get_flags;

extern il2cpp_property_get_get_method_t
        g_il2cpp_property_get_get_method;

extern il2cpp_property_get_set_method_t
        g_il2cpp_property_get_set_method;


// Objects

extern il2cpp_object_get_class_t
        g_il2cpp_object_get_class;

extern il2cpp_object_get_size_t
        g_il2cpp_object_get_size;

extern il2cpp_object_get_virtual_method_t
        g_il2cpp_object_get_virtual_method;

extern il2cpp_object_unbox_t
        g_il2cpp_object_unbox;

extern il2cpp_object_new_t
        g_il2cpp_object_new;


// Runtime

extern il2cpp_runtime_invoke_t
        g_il2cpp_runtime_invoke;

extern il2cpp_runtime_invoke_convert_args_t
        g_il2cpp_runtime_invoke_convert_args;

extern il2cpp_runtime_class_init_t
        g_il2cpp_runtime_class_init;

extern il2cpp_runtime_object_init_t
        g_il2cpp_runtime_object_init;

extern il2cpp_runtime_object_init_exception_t
        g_il2cpp_runtime_object_init_exception;

extern il2cpp_value_box_t
        g_il2cpp_value_box;


// =========================================================
// INITIALIZATION
// =========================================================

bool LoadIl2CppAPI();


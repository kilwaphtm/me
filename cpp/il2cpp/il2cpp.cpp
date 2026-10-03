#include "il2cpp.h"

#include <android/log.h>
#include <dlfcn.h>



void* g_Il2CppHandle = nullptr;


// =========================================================
// DOMAIN / ASSEMBLY / IMAGE
// =========================================================

il2cpp_domain_get_t
        g_il2cpp_domain_get = nullptr;
il2cpp_thread_attach_t
        g_il2cpp_thread_attach = nullptr;

il2cpp_domain_get_assemblies_t
        g_il2cpp_domain_get_assemblies = nullptr;

il2cpp_assembly_get_image_t
        g_il2cpp_assembly_get_image = nullptr;

il2cpp_image_get_name_t
        g_il2cpp_image_get_name = nullptr;

il2cpp_image_get_filename_t
        g_il2cpp_image_get_filename = nullptr;

il2cpp_image_get_class_count_t
        g_il2cpp_image_get_class_count = nullptr;

il2cpp_image_get_class_t
        g_il2cpp_image_get_class = nullptr;


// =========================================================
// CLASS
// =========================================================

il2cpp_class_from_name_t
        g_il2cpp_class_from_name = nullptr;

il2cpp_class_from_type_t
        g_il2cpp_class_from_type = nullptr;

il2cpp_class_get_name_t
        g_il2cpp_class_get_name = nullptr;

il2cpp_class_get_namespace_t
        g_il2cpp_class_get_namespace = nullptr;

il2cpp_class_get_image_t
        g_il2cpp_class_get_image = nullptr;

il2cpp_class_get_parent_t
        g_il2cpp_class_get_parent = nullptr;

il2cpp_class_get_type_t
        g_il2cpp_class_get_type = nullptr;

il2cpp_class_get_element_class_t
        g_il2cpp_class_get_element_class = nullptr;

il2cpp_class_get_assemblyname_t
        g_il2cpp_class_get_assemblyname = nullptr;

il2cpp_class_is_valuetype_t
        g_il2cpp_class_is_valuetype = nullptr;

il2cpp_class_is_enum_t
        g_il2cpp_class_is_enum = nullptr;

il2cpp_class_is_interface_t
        g_il2cpp_class_is_interface = nullptr;

il2cpp_class_is_abstract_t
        g_il2cpp_class_is_abstract = nullptr;

il2cpp_class_is_generic_t
        g_il2cpp_class_is_generic = nullptr;

il2cpp_class_is_inflated_t
        g_il2cpp_class_is_inflated = nullptr;

il2cpp_class_is_blittable_t
        g_il2cpp_class_is_blittable = nullptr;

il2cpp_class_value_size_t
        g_il2cpp_class_value_size = nullptr;

il2cpp_class_instance_size_t
        g_il2cpp_class_instance_size = nullptr;

il2cpp_class_num_fields_t
        g_il2cpp_class_num_fields = nullptr;

il2cpp_class_has_parent_t
        g_il2cpp_class_has_parent = nullptr;

il2cpp_class_is_assignable_from_t
        g_il2cpp_class_is_assignable_from = nullptr;


// =========================================================
// METHODS
// =========================================================

il2cpp_class_get_methods_t
        g_il2cpp_class_get_methods = nullptr;

il2cpp_class_get_method_from_name_t
        g_il2cpp_class_get_method_from_name = nullptr;

il2cpp_method_get_name_t
        g_il2cpp_method_get_name = nullptr;

il2cpp_method_get_class_t
        g_il2cpp_method_get_class = nullptr;

il2cpp_method_get_declaring_type_t
        g_il2cpp_method_get_declaring_type = nullptr;

il2cpp_method_get_flags_t
        g_il2cpp_method_get_flags = nullptr;

il2cpp_method_get_token_t
        g_il2cpp_method_get_token = nullptr;

il2cpp_method_get_param_t
        g_il2cpp_method_get_param = nullptr;

il2cpp_method_get_param_count_t
        g_il2cpp_method_get_param_count = nullptr;

il2cpp_method_get_param_name_t
        g_il2cpp_method_get_param_name = nullptr;

il2cpp_method_get_return_type_t
        g_il2cpp_method_get_return_type = nullptr;

il2cpp_method_is_generic_t
        g_il2cpp_method_is_generic = nullptr;

il2cpp_method_is_inflated_t
        g_il2cpp_method_is_inflated = nullptr;

il2cpp_method_is_instance_t
        g_il2cpp_method_is_instance = nullptr;


// =========================================================
// TYPES
// =========================================================

il2cpp_type_get_name_t
        g_il2cpp_type_get_name = nullptr;

il2cpp_type_get_name_chunked_t
        g_il2cpp_type_get_name_chunked = nullptr;

il2cpp_type_get_type_t
        g_il2cpp_type_get_type = nullptr;

il2cpp_type_get_class_or_element_class_t
        g_il2cpp_type_get_class_or_element_class = nullptr;

il2cpp_type_get_object_t
        g_il2cpp_type_get_object = nullptr;

il2cpp_type_get_reflection_name_t
        g_il2cpp_type_get_reflection_name = nullptr;

il2cpp_type_get_assembly_qualified_name_t
        g_il2cpp_type_get_assembly_qualified_name = nullptr;

il2cpp_type_is_pointer_type_t
        g_il2cpp_type_is_pointer_type = nullptr;

il2cpp_type_is_byref_t
        g_il2cpp_type_is_byref = nullptr;

il2cpp_type_is_static_t
        g_il2cpp_type_is_static = nullptr;

il2cpp_type_equals_t
        g_il2cpp_type_equals = nullptr;


// =========================================================
// FIELDS
// =========================================================

il2cpp_class_get_fields_t
        g_il2cpp_class_get_fields = nullptr;

il2cpp_class_get_field_from_name_t
        g_il2cpp_class_get_field_from_name = nullptr;

il2cpp_field_get_name_t
        g_il2cpp_field_get_name = nullptr;

il2cpp_field_get_type_t
        g_il2cpp_field_get_type = nullptr;

il2cpp_field_get_offset_t
        g_il2cpp_field_get_offset = nullptr;

il2cpp_field_get_parent_t
        g_il2cpp_field_get_parent = nullptr;

il2cpp_field_get_flags_t
        g_il2cpp_field_get_flags = nullptr;

il2cpp_field_get_value_t
        g_il2cpp_field_get_value = nullptr;

il2cpp_field_get_value_object_t
        g_il2cpp_field_get_value_object = nullptr;

il2cpp_field_set_value_t
        g_il2cpp_field_set_value = nullptr;

il2cpp_field_set_value_object_t
        g_il2cpp_field_set_value_object = nullptr;

il2cpp_field_static_get_value_t
        g_il2cpp_field_static_get_value = nullptr;

il2cpp_field_static_set_value_t
        g_il2cpp_field_static_set_value = nullptr;

il2cpp_field_is_literal_t
        g_il2cpp_field_is_literal = nullptr;


// =========================================================
// PROPERTIES
// =========================================================

il2cpp_class_get_properties_t
        g_il2cpp_class_get_properties = nullptr;

il2cpp_class_get_property_from_name_t
        g_il2cpp_class_get_property_from_name = nullptr;

il2cpp_property_get_name_t
        g_il2cpp_property_get_name = nullptr;

il2cpp_property_get_parent_t
        g_il2cpp_property_get_parent = nullptr;

il2cpp_property_get_flags_t
        g_il2cpp_property_get_flags = nullptr;

il2cpp_property_get_get_method_t
        g_il2cpp_property_get_get_method = nullptr;

il2cpp_property_get_set_method_t
        g_il2cpp_property_get_set_method = nullptr;


// =========================================================
// OBJECTS
// =========================================================

il2cpp_object_get_class_t
        g_il2cpp_object_get_class = nullptr;

il2cpp_object_get_size_t
        g_il2cpp_object_get_size = nullptr;

il2cpp_object_get_virtual_method_t
        g_il2cpp_object_get_virtual_method = nullptr;

il2cpp_object_unbox_t
        g_il2cpp_object_unbox = nullptr;

il2cpp_object_new_t
        g_il2cpp_object_new = nullptr;


// =========================================================
// RUNTIME
// =========================================================

il2cpp_runtime_invoke_t
        g_il2cpp_runtime_invoke = nullptr;

il2cpp_runtime_invoke_convert_args_t
        g_il2cpp_runtime_invoke_convert_args = nullptr;

il2cpp_runtime_class_init_t
        g_il2cpp_runtime_class_init = nullptr;

il2cpp_runtime_object_init_t
        g_il2cpp_runtime_object_init = nullptr;

il2cpp_runtime_object_init_exception_t
        g_il2cpp_runtime_object_init_exception = nullptr;

il2cpp_value_box_t
        g_il2cpp_value_box = nullptr;


// =========================================================
// IL2CPP SYMBOL LOADER
// =========================================================

template <typename T>
static bool ResolveIl2CppSymbol(
        T& output,
        const char* name)
{
    output =
            reinterpret_cast<T>(
                    dlsym(
                            g_Il2CppHandle,
                            name
                    )
            );

    if (!output)
    {
        __android_log_print(
                ANDROID_LOG_ERROR,
                "MY_CUSTOM_SO",
                "[IL2CPP] Missing symbol: %s",
                name
        );

        return false;
    }

    return true;
}


// =========================================================
// LOAD IL2CPP API
// =========================================================

bool LoadIl2CppAPI()
{
    if (!g_Il2CppHandle)
    {
        g_Il2CppHandle =
                dlopen(
                        "libil2cpp.so",
                        RTLD_NOW
                );

        if (!g_Il2CppHandle)
        {
            __android_log_print(
                    ANDROID_LOG_INFO,
                    "MY_CUSTOM_SO",
                    "[IL2CPP] libil2cpp.so not loaded yet"
            );

            return false;
        }

        __android_log_print(
                ANDROID_LOG_INFO,
                "MY_CUSTOM_SO",
                "[IL2CPP] libil2cpp.so handle acquired"
        );
    }


    // =====================================================
    // DOMAIN / ASSEMBLY / IMAGE
    // =====================================================

    bool ok = true;


    ok &= ResolveIl2CppSymbol(
            g_il2cpp_domain_get,
            "il2cpp_domain_get"
    );
    ok &= ResolveIl2CppSymbol(
            g_il2cpp_thread_get_all_attached_threads,
            "il2cpp_thread_get_all_attached_threads"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_thread_current,
            "il2cpp_thread_current"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_thread_attach,
            "il2cpp_thread_attach"
    );


    ok &= ResolveIl2CppSymbol(
            g_il2cpp_domain_get_assemblies,
            "il2cpp_domain_get_assemblies"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_assembly_get_image,
            "il2cpp_assembly_get_image"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_image_get_name,
            "il2cpp_image_get_name"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_image_get_filename,
            "il2cpp_image_get_filename"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_image_get_class_count,
            "il2cpp_image_get_class_count"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_image_get_class,
            "il2cpp_image_get_class"
    );


    // =====================================================
    // CLASS
    // =====================================================

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_from_name,
            "il2cpp_class_from_name"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_from_type,
            "il2cpp_class_from_type"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_get_name,
            "il2cpp_class_get_name"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_get_namespace,
            "il2cpp_class_get_namespace"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_get_image,
            "il2cpp_class_get_image"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_get_parent,
            "il2cpp_class_get_parent"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_get_type,
            "il2cpp_class_get_type"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_get_element_class,
            "il2cpp_class_get_element_class"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_get_assemblyname,
            "il2cpp_class_get_assemblyname"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_is_valuetype,
            "il2cpp_class_is_valuetype"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_is_enum,
            "il2cpp_class_is_enum"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_is_interface,
            "il2cpp_class_is_interface"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_is_abstract,
            "il2cpp_class_is_abstract"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_is_generic,
            "il2cpp_class_is_generic"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_is_inflated,
            "il2cpp_class_is_inflated"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_is_blittable,
            "il2cpp_class_is_blittable"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_value_size,
            "il2cpp_class_value_size"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_instance_size,
            "il2cpp_class_instance_size"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_num_fields,
            "il2cpp_class_num_fields"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_has_parent,
            "il2cpp_class_has_parent"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_is_assignable_from,
            "il2cpp_class_is_assignable_from"
    );


    // =====================================================
    // METHODS
    // =====================================================

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_get_methods,
            "il2cpp_class_get_methods"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_get_method_from_name,
            "il2cpp_class_get_method_from_name"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_method_get_name,
            "il2cpp_method_get_name"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_method_get_class,
            "il2cpp_method_get_class"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_method_get_declaring_type,
            "il2cpp_method_get_declaring_type"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_method_get_flags,
            "il2cpp_method_get_flags"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_method_get_token,
            "il2cpp_method_get_token"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_method_get_param,
            "il2cpp_method_get_param"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_method_get_param_count,
            "il2cpp_method_get_param_count"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_method_get_param_name,
            "il2cpp_method_get_param_name"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_method_get_return_type,
            "il2cpp_method_get_return_type"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_method_is_generic,
            "il2cpp_method_is_generic"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_method_is_inflated,
            "il2cpp_method_is_inflated"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_method_is_instance,
            "il2cpp_method_is_instance"
    );


    // =====================================================
    // TYPES
    // =====================================================

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_type_get_name,
            "il2cpp_type_get_name"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_type_get_name_chunked,
            "il2cpp_type_get_name_chunked"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_type_get_type,
            "il2cpp_type_get_type"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_type_get_class_or_element_class,
            "il2cpp_type_get_class_or_element_class"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_type_get_object,
            "il2cpp_type_get_object"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_type_get_reflection_name,
            "il2cpp_type_get_reflection_name"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_type_get_assembly_qualified_name,
            "il2cpp_type_get_assembly_qualified_name"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_type_is_pointer_type,
            "il2cpp_type_is_pointer_type"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_type_is_byref,
            "il2cpp_type_is_byref"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_type_is_static,
            "il2cpp_type_is_static"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_type_equals,
            "il2cpp_type_equals"
    );


    // =====================================================
    // FIELDS
    // =====================================================

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_get_fields,
            "il2cpp_class_get_fields"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_get_field_from_name,
            "il2cpp_class_get_field_from_name"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_field_get_name,
            "il2cpp_field_get_name"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_field_get_type,
            "il2cpp_field_get_type"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_field_get_offset,
            "il2cpp_field_get_offset"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_field_get_parent,
            "il2cpp_field_get_parent"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_field_get_flags,
            "il2cpp_field_get_flags"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_field_get_value,
            "il2cpp_field_get_value"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_field_get_value_object,
            "il2cpp_field_get_value_object"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_field_set_value,
            "il2cpp_field_set_value"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_field_set_value_object,
            "il2cpp_field_set_value_object"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_field_static_get_value,
            "il2cpp_field_static_get_value"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_field_static_set_value,
            "il2cpp_field_static_set_value"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_field_is_literal,
            "il2cpp_field_is_literal"
    );


    // =====================================================
    // PROPERTIES
    // =====================================================

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_get_properties,
            "il2cpp_class_get_properties"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_get_property_from_name,
            "il2cpp_class_get_property_from_name"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_property_get_name,
            "il2cpp_property_get_name"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_property_get_parent,
            "il2cpp_property_get_parent"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_property_get_flags,
            "il2cpp_property_get_flags"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_property_get_get_method,
            "il2cpp_property_get_get_method"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_property_get_set_method,
            "il2cpp_property_get_set_method"
    );


    // =====================================================
    // OBJECTS
    // =====================================================

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_object_get_class,
            "il2cpp_object_get_class"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_object_get_size,
            "il2cpp_object_get_size"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_object_get_virtual_method,
            "il2cpp_object_get_virtual_method"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_object_unbox,
            "il2cpp_object_unbox"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_object_new,
            "il2cpp_object_new"
    );


    // =====================================================
    // RUNTIME
    // =====================================================

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_runtime_invoke,
            "il2cpp_runtime_invoke"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_runtime_invoke_convert_args,
            "il2cpp_runtime_invoke_convert_args"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_runtime_class_init,
            "il2cpp_runtime_class_init"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_runtime_object_init,
            "il2cpp_runtime_object_init"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_runtime_object_init_exception,
            "il2cpp_runtime_object_init_exception"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_value_box,
            "il2cpp_value_box"
    );


    // =====================================================
    // RESULT
    // =====================================================

    if (!ok)
    {
        __android_log_print(
                ANDROID_LOG_ERROR,
                "MY_CUSTOM_SO",
                "[IL2CPP] API incomplete"
        );

        return false;
    }


    __android_log_print(
            ANDROID_LOG_INFO,
            "MY_CUSTOM_SO",
            "[IL2CPP] API loaded successfully"
    );

    return true;
}

il2cpp_thread_get_all_attached_threads_t
        g_il2cpp_thread_get_all_attached_threads = nullptr;

il2cpp_thread_current_t
        g_il2cpp_thread_current = nullptr;


#include "il2cpp.h"
#include <android/log.h>
#include <dlfcn.h>

void* g_Il2CppHandle = nullptr;

il2cpp_domain_get_t
        g_il2cpp_domain_get = nullptr;

il2cpp_domain_get_assemblies_t
        g_il2cpp_domain_get_assemblies = nullptr;

il2cpp_assembly_get_image_t
        g_il2cpp_assembly_get_image = nullptr;

il2cpp_image_get_name_t
        g_il2cpp_image_get_name = nullptr;

il2cpp_class_from_name_t
        g_il2cpp_class_from_name = nullptr;

il2cpp_class_get_method_from_name_t
        g_il2cpp_class_get_method_from_name = nullptr;

il2cpp_runtime_invoke_t
        g_il2cpp_runtime_invoke = nullptr;

il2cpp_object_unbox_t
        g_il2cpp_object_unbox = nullptr;
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
    if (
            g_il2cpp_domain_get &&
            g_il2cpp_domain_get_assemblies &&
            g_il2cpp_assembly_get_image &&
            g_il2cpp_image_get_name &&
            g_il2cpp_class_from_name &&
            g_il2cpp_class_get_method_from_name &&
            g_il2cpp_runtime_invoke &&
            g_il2cpp_object_unbox
            )
    {
        return true;
    }


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


    bool ok = true;


    ok &= ResolveIl2CppSymbol(
            g_il2cpp_domain_get,
            "il2cpp_domain_get"
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
            g_il2cpp_class_from_name,
            "il2cpp_class_from_name"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_class_get_method_from_name,
            "il2cpp_class_get_method_from_name"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_runtime_invoke,
            "il2cpp_runtime_invoke"
    );

    ok &= ResolveIl2CppSymbol(
            g_il2cpp_object_unbox,
            "il2cpp_object_unbox"
    );


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
            "[IL2CPP] API loaded"
    );

    return true;
}
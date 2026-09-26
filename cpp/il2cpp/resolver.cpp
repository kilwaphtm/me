#include "resolver.h"

#include <android/log.h>
#include <cstring>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, "MY_CUSTOM_SO", __VA_ARGS__)

Il2CppImage* g_InputImage = nullptr;
Il2CppClass* g_InputClass = nullptr;

const MethodInfo* g_GetTouchMethod = nullptr;
const MethodInfo* g_GetTouchCountMethod = nullptr;
const MethodInfo* g_GetMouseButtonMethod = nullptr;

GetTouchCountFn g_OriginalGetTouchCount = nullptr;
GetMouseButtonFn g_OriginalGetMouseButton = nullptr;

void* g_GetTouchCountStub = nullptr;
void* g_GetMouseButtonStub = nullptr;
// =========================================================
// STRING MATCH
// =========================================================

static bool ImageNameMatches(
        const char* name,
        const char* wanted)
{
    if (!name || !wanted)
        return false;

    if (strcmp(name, wanted) == 0)
        return true;

    const size_t len = strlen(name);

    if (
            len > 4 &&
            strcmp(name + len - 4, ".dll") == 0
            )
    {
        if (
                strncmp(
                        name,
                        wanted,
                        len - 4
                ) == 0
                )
        {
            return true;
        }
    }

    return false;
}


// =========================================================
// FIND IMAGE
// =========================================================

Il2CppImage* FindImage(
        const char* imageName)
{
    if (!imageName)
        return nullptr;

    if (!g_il2cpp_domain_get ||
        !g_il2cpp_domain_get_assemblies ||
        !g_il2cpp_assembly_get_image ||
        !g_il2cpp_image_get_name)
    {
        LOGI("[RESOLVER] IL2CPP API not ready");

        return nullptr;
    }

    Il2CppDomain* domain =
            g_il2cpp_domain_get();

    if (!domain)
    {
        LOGI("[RESOLVER] Domain not ready");

        return nullptr;
    }

    size_t assemblyCount = 0;

    Il2CppAssembly** assemblies =
            g_il2cpp_domain_get_assemblies(
                    domain,
                    &assemblyCount
            );

    if (!assemblies || assemblyCount == 0)
    {
        LOGI("[RESOLVER] Assemblies not ready");

        return nullptr;
    }

    LOGI(
            "[RESOLVER] Searching image: %s",
            imageName
    );

    for (
            size_t i = 0;
            i < assemblyCount;
            ++i
            )
    {
        if (!assemblies[i])
            continue;

        Il2CppImage* image =
                g_il2cpp_assembly_get_image(
                        assemblies[i]
                );

        if (!image)
            continue;

        const char* name =
                g_il2cpp_image_get_name(
                        image
                );

        if (!name)
            continue;

        if (ImageNameMatches(name, imageName))
        {
            LOGI(
                    "[RESOLVER] IMAGE FOUND: %s",
                    name
            );

            return image;
        }
    }

    LOGI(
            "[RESOLVER] IMAGE NOT FOUND: %s",
            imageName
    );

    return nullptr;
}


// =========================================================
// FIND UNITY INPUT IMAGE
// =========================================================

Il2CppImage* FindUnityInputImage()
{
    // First try the common Unity input assembly.
    Il2CppImage* image =
            FindImage("UnityEngine.InputLegacyModule");

    if (image)
    {
        LOGI(
                "[IL2CPP] FOUND INPUT IMAGE: UnityEngine.InputLegacyModule"
        );

        return image;
    }

    // Some Unity builds place UnityEngine.Input
    // directly inside UnityEngine.dll.
    image = FindImage("UnityEngine");

    if (image)
    {
        Il2CppClass* testClass =
                g_il2cpp_class_from_name(
                        image,
                        "UnityEngine",
                        "Input"
                );

        if (testClass)
        {
            LOGI(
                    "[IL2CPP] FOUND Input class inside: UnityEngine"
            );

            return image;
        }
    }

    LOGI(
            "[IL2CPP] Unity Input image not ready"
    );

    return nullptr;
}

// =========================================================
// FIND UNITY INPUT METHODS
// =========================================================

bool FindUnityInputMethods()
{
    if (
            g_InputClass &&
            g_GetTouchCountMethod &&
            g_GetMouseButtonMethod &&
            g_GetTouchMethod
            )
    {
        return true;
    }

    if (!g_InputImage)
    {
        g_InputImage =
                FindUnityInputImage();

        if (!g_InputImage)
            return false;
    }

    if (!g_InputClass)
    {
        g_InputClass =
                g_il2cpp_class_from_name(
                        g_InputImage,
                        "UnityEngine",
                        "Input"
                );

        if (!g_InputClass)
        {
            LOGI(
                    "[INPUT] UnityEngine.Input class not ready"
            );

            return false;
        }

        LOGI(
                "[INPUT] UnityEngine.Input class FOUND"
        );
    }

    if (!g_GetTouchCountMethod)
    {
        g_GetTouchCountMethod =
                g_il2cpp_class_get_method_from_name(
                        g_InputClass,
                        "get_touchCount",
                        0
                );

        if (g_GetTouchCountMethod)
        {
            LOGI(
                    "[INPUT] get_touchCount FOUND"
            );
        }
    }

    if (!g_GetMouseButtonMethod)
    {
        g_GetMouseButtonMethod =
                g_il2cpp_class_get_method_from_name(
                        g_InputClass,
                        "GetMouseButton",
                        1
                );

        if (g_GetMouseButtonMethod)
        {
            LOGI(
                    "[INPUT] GetMouseButton FOUND"
            );
        }
    }

    if (!g_GetTouchMethod)
    {
        g_GetTouchMethod =
                g_il2cpp_class_get_method_from_name(
                        g_InputClass,
                        "GetTouch",
                        1
                );

        if (g_GetTouchMethod)
        {
            LOGI(
                    "[INPUT] GetTouch FOUND"
            );
        }
    }

    if (
            !g_GetTouchCountMethod ||
            !g_GetMouseButtonMethod ||
            !g_GetTouchMethod
            )
    {
        LOGI(
                "[INPUT] Waiting for Unity Input methods..."
        );

        return false;
    }

    LOGI(
            "[INPUT] ALL Unity Input methods FOUND"
    );

    return true;
}

// =========================================================
// GET UNITY TOUCH
// =========================================================

bool GetUnityTouch(
        int index,
        UnityTouch& output)
{
    if (
            !g_GetTouchMethod ||
            !g_il2cpp_runtime_invoke ||
            !g_il2cpp_object_unbox
            )
    {
        return false;
    }


    void* args[1];

    args[0] = &index;


    Il2CppObject* exception = nullptr;


    Il2CppObject* result =
            g_il2cpp_runtime_invoke(
                    g_GetTouchMethod,
                    nullptr,
                    args,
                    &exception
            );


    if (exception)
        return false;


    if (!result)
        return false;


    void* unboxed =
            g_il2cpp_object_unbox(
                    result
            );


    if (!unboxed)
        return false;


    memcpy(
            &output,
            unboxed,
            sizeof(UnityTouch)
    );


    return true;
}
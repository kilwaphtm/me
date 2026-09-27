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
// FIND CLASS AUTOMATICALLY
// =========================================================
Il2CppClass* FindClassAuto(
        const char* className)
{
    if (!className)
        return nullptr;

    if (!g_il2cpp_domain_get ||
        !g_il2cpp_domain_get_assemblies ||
        !g_il2cpp_assembly_get_image ||
        !g_il2cpp_image_get_class_count ||
        !g_il2cpp_image_get_class ||
        !g_il2cpp_class_get_name ||
        !g_il2cpp_class_get_namespace)
    {
        LOGI("[RESOLVER] Class enumeration API not ready");
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
            "[RESOLVER] Searching class: %s",
            className
    );

    Il2CppClass* foundClass = nullptr;

    int matchCount = 0;

    for (size_t i = 0; i < assemblyCount; ++i)
    {
        if (!assemblies[i])
            continue;

        Il2CppImage* image =
                g_il2cpp_assembly_get_image(
                        assemblies[i]
                );

        if (!image)
            continue;

        const char* imageName =
                g_il2cpp_image_get_name(
                        image
                );

        size_t classCount =
                g_il2cpp_image_get_class_count(
                        image
                );

        for (size_t j = 0; j < classCount; ++j)
        {
            Il2CppClass* klass =
                    g_il2cpp_image_get_class(
                            image,
                            j
                    );

            if (!klass)
                continue;

            const char* currentClassName =
                    g_il2cpp_class_get_name(
                            klass
                    );

            if (!currentClassName)
                continue;

            if (strcmp(currentClassName, className) != 0)
                continue;

            const char* namespaceName =
                    g_il2cpp_class_get_namespace(
                            klass
                    );

            ++matchCount;

            LOGI(
                    "[RESOLVER] CLASS MATCH #%d: %s.%s | Image: %s",
                    matchCount,
                    namespaceName ? namespaceName : "",
                    currentClassName,
                    imageName ? imageName : ""
            );

            if (matchCount == 1)
            {
                foundClass = klass;
            }
        }
    }

    if (matchCount == 0)
    {
        LOGI(
                "[RESOLVER] CLASS NOT FOUND: %s",
                className
        );

        return nullptr;
    }

    if (matchCount > 1)
    {
        LOGI(
                "[RESOLVER] MULTIPLE CLASSES FOUND: %s (%d matches)",
                className,
                matchCount
        );

        return nullptr;
    }

    LOGI(
            "[RESOLVER] CLASS FOUND: %s",
            className
    );

    return foundClass;
}
// =========================================================
// FIND CLASS
// =========================================================

Il2CppClass* FindClass(
        const char* imageName,
        const char* namespaceName,
        const char* className)
{
    if (!imageName || !namespaceName || !className)
        return nullptr;

    if (!g_il2cpp_class_from_name)
    {
        LOGI("[RESOLVER] class_from_name API not ready");
        return nullptr;
    }

    Il2CppImage* image = FindImage(imageName);

    if (!image)
    {
        LOGI(
                "[RESOLVER] Cannot find image for class: %s",
                className
        );

        return nullptr;
    }

    Il2CppClass* klass =
            g_il2cpp_class_from_name(
                    image,
                    namespaceName,
                    className
            );

    if (!klass)
    {
        LOGI(
                "[RESOLVER] CLASS NOT FOUND: %s.%s",
                namespaceName,
                className
        );

        return nullptr;
    }

    LOGI(
            "[RESOLVER] CLASS FOUND: %s.%s",
            namespaceName,
            className
    );

    return klass;
}
// =========================================================
// FIND METHOD
// =========================================================

const MethodInfo* FindMethod(
        const char* className,
        const char* methodName)
{
    if (!className || !methodName)
        return nullptr;

    if (!g_il2cpp_class_get_method_from_name)
    {
        LOGI(
                "[RESOLVER] class_get_method_from_name API not ready"
        );

        return nullptr;
    }

    Il2CppClass* klass =
            FindClassAuto(className);

    if (!klass)
    {
        LOGI(
                "[RESOLVER] Cannot find class: %s",
                className
        );

        return nullptr;
    }

    const MethodInfo* method =
            g_il2cpp_class_get_method_from_name(
                    klass,
                    methodName,
                    -1
            );

    if (!method)
    {
        LOGI(
                "[RESOLVER] METHOD NOT FOUND: %s.%s)",
                className,
                methodName

        );

        return nullptr;
    }

    LOGI(
            "[RESOLVER] METHOD FOUND: %s.%s)",
            className,
            methodName

    );

    return method;
}
// =========================================================
bool GetMethodInfoData(
        const MethodInfo* method,
        MethodInfoData& output)
{
    if (!method)
        return false;
    output.methodName = g_il2cpp_method_get_name(method);
    if (!g_il2cpp_method_get_param_count ||
        !g_il2cpp_method_get_return_type ||
        !g_il2cpp_method_is_generic ||
        !g_il2cpp_method_is_inflated ||
        !g_il2cpp_method_is_instance)
    {
        LOGI(
                "[RESOLVER] Method metadata API not ready"
        );

        return false;
    }

    output.method = method;

    output.address =
            method->methodPointer;

    output.parameterCount =
            g_il2cpp_method_get_param_count(
                    method
            );
    output.parameterTypes.clear();
    output.parameterNames.clear();

    for (uint32_t i = 0; i < output.parameterCount; i++)
    {
        const Il2CppType* paramType =
                g_il2cpp_method_get_param(
                        method,
                        i
                );

        const char* paramName =
                g_il2cpp_method_get_param_name(
                        method,
                        i
                );

        output.parameterTypes.push_back(paramType);
        output.parameterNames.push_back(paramName);
    }

    output.returnType =
            g_il2cpp_method_get_return_type(
                    method
            );

    output.isGeneric =
            g_il2cpp_method_is_generic(
                    method
            );

    output.isInflated =
            g_il2cpp_method_is_inflated(
                    method
            );

    output.isInstance =
            g_il2cpp_method_is_instance(
                    method
            );

    return true;
}
// =========================================================
bool ResolveMethod(
        const char* className,
        const char* methodName,
        MethodInfoData& output,
        bool printInfo)
{
    const MethodInfo* method =
            FindMethod(
                    className,
                    methodName
            );

    if (!method)
        return false;

    if (!GetMethodInfoData(
            method,
            output))
    {
        return false;
    }
    output.className = className;
    output.resultKey = output.className + "::" + output.methodName;
    if (printInfo)
    {
        LOGI("[RESOLVER] ===== METHOD INFO =====");

        LOGI("[RESOLVER] Class  : %s",
             className ? className : "<null>");

        LOGI("[RESOLVER] Method : %s",
             methodName ? methodName : "<null>");

        LOGI("[RESOLVER] Address: %p",
             output.address);

        LOGI("[RESOLVER] Parameter Count: %u",
             output.parameterCount);

        for (uint32_t i = 0; i < output.parameterCount; i++)
        {
            const char* typeName =
                    g_il2cpp_type_get_name(
                            output.parameterTypes[i]
                    );

            const char* paramName =
                    output.parameterNames[i];

            LOGI(
                    "[RESOLVER] Param[%u] name=%s type=%s",
                    i,
                    paramName ? paramName : "<null>",
                    typeName ? typeName : "<null>"
            );
        }

        const char* returnTypeName =
                g_il2cpp_type_get_name(
                        output.returnType
                );

        LOGI("[RESOLVER] Return Type: %s",
             returnTypeName
             ? returnTypeName
             : "<null>");

        LOGI("[RESOLVER] Is Generic  : %s",
             output.isGeneric ? "true" : "false");

        LOGI("[RESOLVER] Is Inflated : %s",
             output.isInflated ? "true" : "false");

        LOGI("[RESOLVER] Is Instance  : %s",
             output.isInstance ? "true" : "false");

        LOGI("[RESOLVER] =======================");
    }

    return true;
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
            !g_il2cpp_object_unbox ||
            !g_il2cpp_method_get_param ||
            !g_il2cpp_method_get_param_name
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
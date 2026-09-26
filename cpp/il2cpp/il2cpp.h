#pragma once

#include <cstddef>

// =========================================================
// IL2CPP TYPES
// =========================================================

struct Il2CppDomain;
struct Il2CppAssembly;
struct Il2CppImage;
struct Il2CppClass;
struct Il2CppObject;

struct MethodInfo
{
    void* methodPointer;
};


// =========================================================
// IL2CPP FUNCTION TYPES
// =========================================================

using il2cpp_domain_get_t =
        Il2CppDomain* (*)();

using il2cpp_domain_get_assemblies_t =
        Il2CppAssembly** (*)(Il2CppDomain*, size_t*);

using il2cpp_assembly_get_image_t =
        Il2CppImage* (*)(const Il2CppAssembly*);

using il2cpp_image_get_name_t =
        const char* (*)(const Il2CppImage*);

using il2cpp_class_from_name_t =
        Il2CppClass* (*)(const Il2CppImage*, const char*, const char*);

using il2cpp_class_get_method_from_name_t =
        const MethodInfo* (*)(Il2CppClass*, const char*, int);

using il2cpp_runtime_invoke_t =
        Il2CppObject* (*)(const MethodInfo*, void*, void**, Il2CppObject**);

using il2cpp_object_unbox_t =
        void* (*)(Il2CppObject*);


// =========================================================
// IL2CPP API
// =========================================================

extern void* g_Il2CppHandle;

extern il2cpp_domain_get_t
        g_il2cpp_domain_get;

extern il2cpp_domain_get_assemblies_t
        g_il2cpp_domain_get_assemblies;

extern il2cpp_assembly_get_image_t
        g_il2cpp_assembly_get_image;

extern il2cpp_image_get_name_t
        g_il2cpp_image_get_name;

extern il2cpp_class_from_name_t
        g_il2cpp_class_from_name;

extern il2cpp_class_get_method_from_name_t
        g_il2cpp_class_get_method_from_name;

extern il2cpp_runtime_invoke_t
        g_il2cpp_runtime_invoke;

extern il2cpp_object_unbox_t
        g_il2cpp_object_unbox;


// =========================================================
// INITIALIZATION
// =========================================================

bool LoadIl2CppAPI();
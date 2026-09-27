#pragma once
#include <vector>
#include "il2cpp.h"

#include <string>

struct InspectedValue
{
    // =====================================================
    // TYPE INFORMATION
    // =====================================================

    const Il2CppType* type = nullptr;
    Il2CppClass* klass = nullptr;

    int typeKind = -1;

    std::string typeName;

    // =====================================================
    // VALUE STATE
    // =====================================================

    bool isNull = false;
    bool isEnum = false;

    // =====================================================
    // PRIMITIVE VALUES
    // =====================================================

    bool boolValue = false;

    int64_t signedValue = 0;
    uint64_t unsignedValue = 0;

    float floatValue = 0.0f;
    double doubleValue = 0.0;

    // =====================================================
    // REFERENCE / POINTER
    // =====================================================

    void* pointerValue = nullptr;

    // =====================================================
    // STRING
    // =====================================================

    std::string stringValue;

    // =====================================================
    // DISPLAY
    // =====================================================

    std::string displayValue;
};
struct HookResult
{
    std::vector<InspectedValue> arguments;
    InspectedValue returnValue;
};
// =========================================================
// IL2CPP VALUE INSPECTOR
// =========================================================

class Il2CppValueInspector
{
public:
    // يرجع اسم نوع IL2CPP كنص.
    static const char* GetTypeName(const Il2CppType* type);

    // يرجع اسم النوع اعتمادًا على Il2CppTypeEnum.
    static const char* GetTypeKindName(const Il2CppType* type);

    static bool InspectClass(const Il2CppType* type);

    static void InspectEnumValue(const Il2CppType* type,const void* data);
    static InspectedValue InspectValue(
            const Il2CppType* type,
            const void* data
    );
};
// =========================================================
// CLASS INSPECTION
// =========================================================

static bool InspectClass(const Il2CppType* type);


static void InspectEnumValue(
        const Il2CppType* type,
        const void* data
);


static void InspectValue(
        const Il2CppType* type,
        const void* data
);
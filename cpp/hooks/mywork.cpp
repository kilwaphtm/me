#include "mywork.h"
#include "../il2cpp/resolver.h"
#include <android/log.h>
#include "shadowhook.h"
#include "../il2cpp/il2cpp.h"
#include <string>
#include <cstdint>
#include "myhook.h"
#include "../il2cpp/value_inspector.h"

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,"MY_CUSTOM_SO",__VA_ARGS__)




struct Il2CppStringLayout
{
    void* klass;
    void* monitor;
    int32_t length;
    uint16_t chars[1];
};
static std::string GetIl2CppString(void* stringObject)
{
    if (!stringObject)
        return "<null>";

    Il2CppStringLayout* str =
            reinterpret_cast<Il2CppStringLayout*>(
                    stringObject
            );

    if (str->length <= 0)
        return "";

    std::string result;

    result.reserve(
            static_cast<size_t>(str->length)
    );

    for (int32_t i = 0; i < str->length; i++)
    {
        uint16_t c = str->chars[i];

        if (c < 0x80)
        {
            result.push_back(
                    static_cast<char>(c)
            );
        }
        else if (c < 0x800)
        {
            result.push_back(
                    static_cast<char>(
                            0xC0 | (c >> 6)
                    )
            );

            result.push_back(
                    static_cast<char>(
                            0x80 | (c & 0x3F)
                    )
            );
        }
        else
        {
            result.push_back(
                    static_cast<char>(
                            0xE0 | (c >> 12)
                    )
            );

            result.push_back(
                    static_cast<char>(
                            0x80 | ((c >> 6) & 0x3F)
                    )
            );

            result.push_back(
                    static_cast<char>(
                            0x80 | (c & 0x3F)
                    )
            );
        }
    }

    return result;
}

struct BetStateController_o;
void TestResolverUsage()
{
    MethodInfoData data;

    if (!ResolveMethod(
            "BetStateController",
            "get_CurrentBet",
            data,
            true))
    {
        LOGI("[MYWORK] ResolveMethod FAILED");
        return;
    }

    LOGI("[MYWORK] ResolveMethod SUCCESS");
    LOGI(
            "[INSPECTOR] Return Type: %s | Kind: %s",
            Il2CppValueInspector::GetTypeName(data.returnType),
            Il2CppValueInspector::GetTypeKindName(data.returnType)
    );

    for (uint32_t i = 0; i < data.parameterCount; ++i)
    {
        const char* name =
                (i < data.parameterNames.size() && data.parameterNames[i])
                ? data.parameterNames[i]
                : "<unnamed>";

        const Il2CppType* type =
                (i < data.parameterTypes.size())
                ? data.parameterTypes[i]
                : nullptr;

        LOGI(
                "[INSPECTOR] Param[%u]: name=%s type=%s kind=%s",
                i,
                name,
                Il2CppValueInspector::GetTypeName(type),
                Il2CppValueInspector::GetTypeKindName(type)
        );
    }

    using GetCurrentBet_t =
            int32_t (*)(
                    BetStateController_o*,
                    const MethodInfo*
            );

    static MyHook<GetCurrentBet_t> hook(data);

    if (hook.Monitor())
    {
        LOGI("[MYWORK2] Monitor SUCCESS");

        HookResult* result =
                HookResultManager::Instance().Get(
                        data.resultKey
                );

        if (result)
        {
            LOGI(
                    "[RESULT MANAGER] FOUND: %s | Result=%p",
                    data.resultKey.c_str(),
                    result
            );
        }
        else
        {
            LOGI(
                    "[RESULT MANAGER] NOT FOUND: %s",
                    data.resultKey.c_str()
            );
        }
    }
    else
    {
        LOGI("[MYWORK] Monitor FAILED");
    }
}

void TestResolverUsage2()
{
    MethodInfoData data;

    if (!ResolveMethod(
            "SlotMachineManager",
            "GetSpinResultSymbolsAnalyticFormat",
            data,
            true))
    {
        LOGI("[MYWORK2] ResolveMethod FAILED");
        return;
    }

    LOGI("[MYWORK2] ResolveMethod SUCCESS");

    LOGI(
            "[INSPECTOR] Return Type: %s | Kind: %s",
            Il2CppValueInspector::GetTypeName(data.returnType),
            Il2CppValueInspector::GetTypeKindName(data.returnType)
    );

    for (uint32_t i = 0; i < data.parameterCount; ++i)
    {
        const char* name =
                (i < data.parameterNames.size() && data.parameterNames[i])
                ? data.parameterNames[i]
                : "<unnamed>";

        const Il2CppType* type =
                (i < data.parameterTypes.size())
                ? data.parameterTypes[i]
                : nullptr;

        LOGI(
                "[INSPECTOR] Param[%u]: name=%s type=%s kind=%s",
                i,
                name,
                Il2CppValueInspector::GetTypeName(type),
                Il2CppValueInspector::GetTypeKindName(type)
        );
    }
    if (data.parameterCount > 0)
    {
        LOGI("[MYWORK2] Inspecting Param[0] class...");

        Il2CppValueInspector::InspectClass(
                data.parameterTypes[0]
        );
        int32_t testEnumValue = 5;

        Il2CppValueInspector::InspectEnumValue(
                data.parameterTypes[0],
                &testEnumValue
        );
    }
    using GetSpinResultSymbolsAnalyticFormat_t =
            void* (*)(
                    void*,
                    int32_t,
                    int32_t,
                    int32_t,
                    const MethodInfo*
            );

    static MyHook<GetSpinResultSymbolsAnalyticFormat_t> hook(data);

    if (hook.Monitor())
    {
        LOGI("[MYWORK2] Monitor SUCCESS");
        HookResult* result =
                HookResultManager::Instance().Get(
                        data.resultKey
                );

        if (result)
        {
            LOGI(
                    "[RESULT MANAGER] FOUND: %s | Result=%p",
                    data.resultKey.c_str(),
                    result
            );
        }
        else
        {
            LOGI(
                    "[RESULT MANAGER] NOT FOUND: %s",
                    data.resultKey.c_str()
            );
        }
    }
    else
    {
        LOGI("[MYWORK2] Monitor FAILED");
    }
}
#pragma once
#include <vector>
#include <cstdint>
#include <type_traits>
#include <android/log.h>
#include <unordered_map>
#include "shadowhook.h"
#include "../il2cpp/resolver.h"

#include <utility>
#include "../il2cpp/value_inspector.h"
#define MYHOOK_LOG_TAG "MY_CUSTOM_SO"

#define MYHOOK_LOGI(...) \
    __android_log_print(ANDROID_LOG_INFO, MYHOOK_LOG_TAG, __VA_ARGS__)

#define MYHOOK_LOGE(...) \
    __android_log_print(ANDROID_LOG_ERROR, MYHOOK_LOG_TAG, __VA_ARGS__)


/*
=========================================================
    Forward declaration
=========================================================
*/

template<typename Signature>
class MyHook;


/*
=========================================================
    Function Pointer Signature

    Example:

    using MyMethod_t = int32_t (*)(
        BetStateController_o*,
        const MethodInfo*
    );

    MyHook<MyMethod_t> hook(data);
=========================================================
*/
class HookResultManager
{
public:
    static HookResultManager& Instance()
    {
        static HookResultManager instance;
        return instance;
    }

    void Register(const std::string& key, HookResult* result)
    {
        m_Results[key] = result;
    }

    HookResult* Get(const std::string& key)
    {
        auto it = m_Results.find(key);

        if (it == m_Results.end())
            return nullptr;

        return it->second;
    }

private:
    std::unordered_map<std::string, HookResult*> m_Results;
};

template<typename Return, typename... Args>
class MyHook<Return (*)(Args...)>
{
public:

    using Signature = Return (*)(Args...);
    const HookResult& GetLastResult() const
    {
        return m_LastResult;
    }

public:

    explicit MyHook(const MethodInfoData& data) : m_Data(data)
    {
        HookResultManager::Instance().Register(
                m_Data.resultKey,
                &m_LastResult
        );
    }


    /*
    =====================================================
        Monitor

        Installs hook and calls original automatically.

        No arguments required.
    =====================================================
    */

    bool Monitor();


    /*
    =====================================================
        Execute

        Calls original function directly.

        Arguments must exactly match Signature.
    =====================================================
    */

    Return Execute(Args... args);


    /*
    =====================================================
        Replace

        Replaces original behavior with user function.

        Example:

        hook.Replace(MyReplacement);
    =====================================================
    */

    bool Replace(Signature replacement);


    /*
    =====================================================
        Remove

        Removes the hook.
    =====================================================
    */

    bool Remove();


    /*
    =====================================================
        Status
    =====================================================
    */

    bool IsHooked() const
    {
        return m_IsHooked;
    }


    /*
    =====================================================
        Original

        Returns the original function pointer.

        This is mainly useful for advanced cases.
    =====================================================
    */

    Signature GetOriginal() const
    {
        return m_Original;
    }


private:



    /*
    =====================================================
        Method information
    =====================================================
    */

    MethodInfoData m_Data{};


    /*
    =====================================================
        ShadowHook original function

        IMPORTANT:

        This is EXACTLY the same function pointer type
        supplied by the user.

        Example:

        int32_t (*)(
            BetStateController_o*,
            const MethodInfo*
        );
    =====================================================
    */

    Signature m_Original = nullptr;


    /*
    =====================================================
        ShadowHook stub
    =====================================================
    */

    void* m_Stub = nullptr;


    /*
    =====================================================
        Current replacement

        Used by Replace().
    =====================================================
    */

    Signature m_Replacement = nullptr;


    /*
    =====================================================
        State
    =====================================================
    */

    bool m_IsHooked = false;


    /*
    =====================================================
        Active hook instance

        The generated thunk needs to know which MyHook
        object it belongs to.

        One active hook per Signature.
    =====================================================
    */
    HookResult m_LastResult{};



    static inline MyHook* s_Instance = nullptr;

    template <size_t... I>
    static std::vector<InspectedValue> InspectArguments(
            std::index_sequence<I...>,
            Args... args)
    {
        std::vector<InspectedValue> values;

        (
                [&]()
                {
                    size_t parameterIndex = I;

                    // Instance method:
                    // Args[0] = this
                    // Args[1...] = declared parameters
                    if (s_Instance->m_Data.isInstance)
                    {
                        if (I == 0)
                            return;

                        parameterIndex = I - 1;
                    }

                    // Ignore hidden MethodInfo* at the end.
                    if (parameterIndex >=
                        s_Instance->m_Data.parameterTypes.size())
                    {
                        return;
                    }

                    values.push_back(
                            Il2CppValueInspector::InspectValue(
                                    s_Instance->m_Data.parameterTypes[parameterIndex],
                                    static_cast<const void*>(&args)
                            )
                    );
                }(),
                ...
        );

        return values;
    }
    /*
    =====================================================
        Monitor thunk

        IMPORTANT:

        Because this class is specialized using:

            Return (*)(Args...)

        this function is generated with the EXACT same
        native function signature.

        Example:

            int32_t MonitorThunk(
                BetStateController_o*,
                const MethodInfo*
            );

        Therefore ShadowHook receives an actual function
        with the same ABI/signature as the target.
    =====================================================
    */

    static Return MonitorThunk(Args... args)
    {
        MyHook* hook = s_Instance;

        if (!hook)
        {
            MYHOOK_LOGE(
                    "[MYHOOK] MonitorThunk: instance is NULL"
            );

            if constexpr (std::is_void_v<Return>)
            {
                return;
            }
            else
            {
                return Return{};
            }
        }


        MYHOOK_LOGI(
                "[MYHOOK] METHOD CALLED: %p",
                hook->m_Data.address
        );


        if (hook->m_Replacement)
        {
            MYHOOK_LOGI(
                    "[MYHOOK] USING REPLACEMENT"
            );

            return hook->m_Replacement(args...);
        }


        if (!hook->m_Original)
        {
            MYHOOK_LOGE(
                    "[MYHOOK] ORIGINAL IS NULL"
            );

            if constexpr (std::is_void_v<Return>)
            {
                return;
            }
            else
            {
                return Return{};
            }
        }

        hook->m_LastResult.arguments =
                InspectArguments(
                        std::index_sequence_for<Args...>{},
                        args...
                );
        MYHOOK_LOGI(
                "[MYHOOK] BEFORE ORIGINAL"
        );



        if constexpr (std::is_void_v<Return>)
        {
            hook->m_Original(args...);
            hook->m_LastResult.hasNewResult = true;
            MYHOOK_LOGI(
                    "[MYHOOK] AFTER ORIGINAL"
            );

            return;
        }
        else
        {
            Return result =
                    hook->m_Original(args...);

            MYHOOK_LOGI(
                    "[MYHOOK] AFTER ORIGINAL"
            );

            if (hook->m_Data.returnType)
            {
                hook->m_LastResult.returnValue =
                        Il2CppValueInspector::InspectValue(
                                hook->m_Data.returnType,
                                &result
                        );
                hook->m_LastResult.hasNewResult = true;
            }

            return result;
        }
    }
};


/*
=========================================================
    Monitor
=========================================================
*/

template<typename Return, typename... Args>
bool MyHook<Return (*)(Args...)>::Monitor()
{
    /*
    =====================================================
        Validate address
    =====================================================
    */

    if (!m_Data.address)
    {
        MYHOOK_LOGE(
                "[MYHOOK] Monitor FAILED: address is NULL"
        );

        return false;
    }


    /*
    =====================================================
        Already hooked
    =====================================================
    */

    if (m_IsHooked)
    {
        MYHOOK_LOGI(
                "[MYHOOK] Already hooked"
        );

        return true;
    }


    /*
    =====================================================
        Only one active hook for this generated thunk
    =====================================================
    */

    if (s_Instance)
    {
        MYHOOK_LOGE(
                "[MYHOOK] Monitor FAILED: another hook with "
                "the same Signature is already active"
        );

        return false;
    }


    /*
    =====================================================
        Install ShadowHook
    =====================================================
    */

    void* stub = shadowhook_hook_func_addr(
            m_Data.address,

            reinterpret_cast<void*>(
                    &MonitorThunk
            ),

            reinterpret_cast<void**>(
                    &m_Original
            )
    );


    /*
    =====================================================
        Check result
    =====================================================
    */

    if (!stub)
    {
        MYHOOK_LOGE(
                "[MYHOOK] Monitor FAILED: ShadowHook returned NULL"
        );

        m_Original = nullptr;

        return false;
    }


    /*
    =====================================================
        Save state
    =====================================================
    */

    m_Stub = stub;

    s_Instance = this;

    m_IsHooked = true;


    MYHOOK_LOGI(
            "[MYHOOK] Monitor SUCCESS: "
            "address=%p original=%p stub=%p",
            m_Data.address,
            reinterpret_cast<void*>(
                    m_Original
            ),
            m_Stub
    );


    return true;
}


/*
=========================================================
    Execute
=========================================================
*/

template<typename Return, typename... Args>
Return MyHook<Return (*)(Args...)>::Execute(
        Args... args
)
{
    if (!m_Original)
    {
        MYHOOK_LOGE(
                "[MYHOOK] Execute FAILED: original is NULL"
        );

        if constexpr (!std::is_void_v<Return>)
            return Return{};

        return;
    }


    MYHOOK_LOGI(
            "[MYHOOK] Execute ORIGINAL"
    );


    return m_Original(
            args...
    );
}


/*
=========================================================
    Replace
=========================================================
*/

template<typename Return, typename... Args>
bool MyHook<Return (*)(Args...)>::Replace(
        Signature replacement
)
{
    if (!m_Data.address)
    {
        MYHOOK_LOGE(
                "[MYHOOK] Replace FAILED: address is NULL"
        );

        return false;
    }


    if (!replacement)
    {
        MYHOOK_LOGE(
                "[MYHOOK] Replace FAILED: replacement is NULL"
        );

        return false;
    }


    /*
    =====================================================
        Make sure hook exists first
    =====================================================
    */

    if (!m_IsHooked)
    {
        MYHOOK_LOGI(
                "[MYHOOK] Replace: hook not installed, "
                "installing Monitor first"
        );

        if (!Monitor())
        {
            MYHOOK_LOGE(
                    "[MYHOOK] Replace FAILED: Monitor failed"
            );

            return false;
        }
    }


    /*
    =====================================================
        Save replacement

        MonitorThunk() will automatically call it.
    =====================================================
    */

    m_Replacement = replacement;


    MYHOOK_LOGI(
            "[MYHOOK] Replace SUCCESS"
    );


    return true;
}


/*
=========================================================
    Remove
=========================================================
*/

template<typename Return, typename... Args>
bool MyHook<Return (*)(Args...)>::Remove()
{
    if (!m_IsHooked)
    {
        MYHOOK_LOGI(
                "[MYHOOK] Remove: hook is not active"
        );

        return true;
    }


    /*
    =====================================================
        ShadowHook unhook
    =====================================================

        m_Stub is the stub returned by
        shadowhook_hook_func_addr().
    =====================================================
    */

    if (!m_Stub)
    {
        MYHOOK_LOGE(
                "[MYHOOK] Remove FAILED: stub is NULL"
        );

        return false;
    }


    int result =
            shadowhook_unhook(
                    m_Stub
            );


    if (result != 0)
    {
        MYHOOK_LOGE(
                "[MYHOOK] Remove FAILED: "
                "shadowhook_unhook returned %d",
                result
        );

        return false;
    }


    /*
    =====================================================
        Clear state
    =====================================================
    */

    m_Stub = nullptr;

    m_Original = nullptr;

    m_Replacement = nullptr;

    m_IsHooked = false;


    if (s_Instance == this)
        s_Instance = nullptr;


    MYHOOK_LOGI(
            "[MYHOOK] Remove SUCCESS"
    );


    return true;
}
#include "mywork.h"
#include "../il2cpp/resolver.h"
#include <android/log.h>
#include "shadowhook.h"
#include "../il2cpp/il2cpp.h"
#include <string>
#include <cstdint>
#include "myhook.h"
#include "../il2cpp/value_inspector.h"
#include <thread>
#include <chrono>
#include <unistd.h>
#include <pthread.h>
#include <functional>
#include <mutex>
#include <queue>

#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,"MY_CUSTOM_SO",__VA_ARGS__)

static std::mutex g_ScheduleMutex;
static std::queue<std::function<void()>>
        g_ScheduledCallbacks;
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


static void MainThreadCallback(Il2CppObject* state)
{
    std::function<void()> callback;

    {
        std::lock_guard<std::mutex> lock(
                g_ScheduleMutex
        );

        if (g_ScheduledCallbacks.empty())
        {
            LOGI(
                    "[SCHEDULE] Callback queue is empty"
            );
            return;
        }

        callback =
                std::move(
                        g_ScheduledCallbacks.front()
                );

        g_ScheduledCallbacks.pop();
    }

    LOGI(
            "[SCHEDULE] CALLBACK TID = %d",
            gettid()
    );

    if (callback)
    {
        callback();
    }
}
bool PostToMainThread(
        std::function<void()> callback)
{
    if (!callback)
    {
        LOGI(
                "[SCHEDULE] Callback is empty"
        );

        return false;
    }

    // =====================================================
    // Get UnitySynchronizationContext
    // =====================================================

    Il2CppObject* syncContext =
            GetMainThreadSynchronizationContext();

    if (!syncContext)
    {
        LOGI(
                "[SCHEDULE] SyncContext = NULL"
        );

        return false;
    }

    Il2CppClass* syncClass =
            g_il2cpp_object_get_class(
                    syncContext
            );

    if (!syncClass)
    {
        LOGI(
                "[SCHEDULE] SyncContext class = NULL"
        );

        return false;
    }

    // =====================================================
    // Post(SendOrPostCallback, object)
    // =====================================================

    const MethodInfo* postMethod =
            g_il2cpp_class_get_method_from_name(
                    syncClass,
                    "Post",
                    2
            );

    if (!postMethod)
    {
        LOGI(
                "[SCHEDULE] Post NOT FOUND"
        );

        return false;
    }

    // =====================================================
    // SendOrPostCallback
    // =====================================================

    Il2CppClass* callbackClass =
            FindClassAuto(
                    "SendOrPostCallback"
            );

    if (!callbackClass)
    {
        LOGI(
                "[SCHEDULE] SendOrPostCallback NOT FOUND"
        );

        return false;
    }

    // =====================================================
    // Allocate delegate
    // =====================================================

    Il2CppObject* delegate =
            g_il2cpp_object_new(
                    callbackClass
            );

    if (!delegate)
    {
        LOGI(
                "[SCHEDULE] Delegate allocation FAILED"
        );

        return false;
    }

    // =====================================================
    // Put callback in queue
    // =====================================================

    {
        std::lock_guard<std::mutex> lock(
                g_ScheduleMutex
        );

        g_ScheduledCallbacks.push(
                std::move(callback)
        );
    }

    // =====================================================
    // Configure delegate
    // =====================================================

    Il2CppDelegate* delegateObject =
            reinterpret_cast<Il2CppDelegate*>(
                    delegate
            );

    void* callbackPointer =
            reinterpret_cast<void*>(
                    &MainThreadCallback
            );

    delegateObject->method_ptr =
            reinterpret_cast<Il2CppMethodPointer>(
                    callbackPointer
            );

    delegateObject->invoke_impl =
            reinterpret_cast<Il2CppMethodPointer>(
                    callbackPointer
            );

    delegateObject->target =
            nullptr;

    delegateObject->method =
            nullptr;

    delegateObject->delegate_trampoline =
            nullptr;

    delegateObject->extraArg =
            0;

    delegateObject->invoke_impl_this =
            delegate;

    delegateObject->interp_method =
            nullptr;

    delegateObject->interp_invoke_impl =
            nullptr;

    delegateObject->method_info =
            nullptr;

    delegateObject->original_method_info =
            nullptr;

    delegateObject->data =
            nullptr;

    delegateObject->method_is_virtual =
            false;

    // =====================================================
    // Post(delegate, nullptr)
    // =====================================================

    void* postArgs[2];

    postArgs[0] =
            delegate;

    postArgs[1] =
            nullptr;

    Il2CppObject* exception =
            nullptr;

    LOGI(
            "[SCHEDULE] Posting callback | caller TID=%d",
            gettid()
    );

    g_il2cpp_runtime_invoke(
            postMethod,
            syncContext,
            postArgs,
            &exception
    );

    if (exception)
    {
        LOGI(
                "[SCHEDULE] Post THREW"
        );

        // مهم: لو Post فشل، نشيل الـ callback
        {
            std::lock_guard<std::mutex> lock(
                    g_ScheduleMutex
            );

            if (!g_ScheduledCallbacks.empty())
            {
                g_ScheduledCallbacks.pop();
            }
        }

        return false;
    }

    LOGI(
            "[SCHEDULE] Post SUCCESS"
    );

    return true;
}
Il2CppObject* TestGetInstance(const char* className)
{
    Il2CppClass* klass =
            FindClassAuto(className);

    if (!klass)
    {
        LOGI(
                "[INSTANCE] Class NOT FOUND: %s",
                className
        );
        return nullptr;
    }

    FieldInfo* field =
            g_il2cpp_class_get_field_from_name(
                    klass,
                    "Instance"
            );

    if (!field)
    {
        LOGI(
                "[INSTANCE] Field NOT FOUND: %s::Instance",
                className
        );
        return nullptr;
    }

    Il2CppObject* instance = nullptr;

    g_il2cpp_field_static_get_value(
            field,
            &instance
    );

    if (!instance)
    {
        LOGI(
                "[INSTANCE] %s::Instance = NULL",
                className
        );
        return nullptr;
    }

    LOGI(
            "[INSTANCE] %s::Instance = %p",
            className,
            instance
    );

    Il2CppClass* instanceClass =
            g_il2cpp_object_get_class(instance);

    if (instanceClass)
    {
        LOGI(
                "[INSTANCE] Object Class = %s",
                g_il2cpp_class_get_name(instanceClass)
        );
    }

    return instance;
}
Il2CppObject* GetMainThreadSynchronizationContext()
{
    if (!g_il2cpp_thread_get_all_attached_threads)
    {
        LOGI("[SCHEDULE] get_all_attached_threads = NULL");
        return nullptr;
    }

    size_t count = 0;

    Il2CppThread** threads =
            g_il2cpp_thread_get_all_attached_threads(&count);

    if (!threads || count == 0)
    {
        LOGI("[SCHEDULE] No attached threads");
        return nullptr;
    }

    // نفس Il2Cpp.mainThread في frida-il2cpp-bridge
    Il2CppThread* mainThread = threads[0];

    LOGI(
            "[SCHEDULE] mainThread = %p",
            mainThread
    );

    // bridge يعمل:
    // new Il2Cpp.Object(this)
    //
    // لذلك نتعامل مع Il2CppThread كـ managed object.
    Il2CppObject* threadObject =
            reinterpret_cast<Il2CppObject*>(mainThread);

    if (!threadObject)
    {
        LOGI("[SCHEDULE] threadObject = NULL");
        return nullptr;
    }

    Il2CppClass* threadClass =
            g_il2cpp_object_get_class(threadObject);

    if (!threadClass)
    {
        LOGI("[SCHEDULE] Thread class = NULL");
        return nullptr;
    }

    LOGI(
            "[SCHEDULE] Thread class = %s",
            g_il2cpp_class_get_name(threadClass)
    );

    // -------------------------------------------------
    // GetMutableExecutionContext()
    // -------------------------------------------------

    const MethodInfo* getExecutionContext =
            g_il2cpp_class_get_method_from_name(
                    threadClass,
                    "GetMutableExecutionContext",
                    0
            );

    if (!getExecutionContext)
    {
        LOGI(
                "[SCHEDULE] GetMutableExecutionContext NOT FOUND"
        );
        return nullptr;
    }

    LOGI(
            "[SCHEDULE] GetMutableExecutionContext = %p",
            getExecutionContext
    );

    Il2CppObject* exception = nullptr;

    Il2CppObject* executionContext =
            g_il2cpp_runtime_invoke(
                    getExecutionContext,
                    threadObject,
                    nullptr,
                    &exception
            );

    if (exception)
    {
        LOGI(
                "[SCHEDULE] GetMutableExecutionContext threw exception"
        );
        return nullptr;
    }

    if (!executionContext)
    {
        LOGI(
                "[SCHEDULE] executionContext = NULL"
        );
        return nullptr;
    }

    LOGI(
            "[SCHEDULE] executionContext = %p",
            executionContext
    );

    // -------------------------------------------------
    // _syncContext
    // -------------------------------------------------

    Il2CppClass* executionContextClass =
            g_il2cpp_object_get_class(executionContext);

    if (!executionContextClass)
    {
        LOGI(
                "[SCHEDULE] ExecutionContext class = NULL"
        );
        return nullptr;
    }

    LOGI(
            "[SCHEDULE] ExecutionContext class = %s",
            g_il2cpp_class_get_name(executionContextClass)
    );

    FieldInfo* syncContextField =
            g_il2cpp_class_get_field_from_name(
                    executionContextClass,
                    "_syncContext"
            );

    if (!syncContextField)
    {
        LOGI(
                "[SCHEDULE] _syncContext NOT FOUND"
        );
        return nullptr;
    }

    Il2CppObject* syncContext = nullptr;

    g_il2cpp_field_get_value(
            executionContext,
            syncContextField,
            &syncContext
    );

    LOGI(
            "[SCHEDULE] SynchronizationContext = %p",
            syncContext
    );

    if (syncContext)
    {
        Il2CppClass* syncClass =
                g_il2cpp_object_get_class(syncContext);

        if (syncClass)
        {
            LOGI(
                    "[SCHEDULE] SyncContext class = %s",
                    g_il2cpp_class_get_name(syncClass)
            );
        }
    }

    return syncContext;
}
struct BetStateController_o;
struct SlotMachineManager_o;
struct MoonActive_Raid_RaidAnimationFlowHandler_o;

// -----------------------------------------------

// to monitor >> 1 to active 0 to remove
void get_CurrentBet(int action)
{
    using GetCurrentBet_t =
            int32_t (*)(
                    BetStateController_o*,
                    const MethodInfo*
            );

    static MyHook<GetCurrentBet_t>* hook = nullptr;

    if (action == 1)
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

        hook = new MyHook<GetCurrentBet_t>(data);

        if (hook->Monitor())
        {
            LOGI("[MYWORK] Monitor SUCCESS");
        }
        else
        {
            LOGI("[MYWORK] Monitor FAILED");

            delete hook;
            hook = nullptr;
        }
    }
    else if (action == 0)
    {
        if (!hook)
        {
            LOGI("[MYWORK] Remove: hook is NULL");
            return;
        }

        if (hook->Remove())
        {
            LOGI("[MYWORK] Remove SUCCESS");

            delete hook;
            hook = nullptr;
        }
        else
        {
            LOGI("[MYWORK] Remove FAILED");
        }
    }
}
//=============================
void monitor_set_timeScale(int action)
{
    using m_set_timeScale_t =
            void (*)(
                    float,
                    const MethodInfo*
            );

    static MyHook<m_set_timeScale_t>* hook = nullptr;

    //signedValue
    //unsignedValue
    //floatValue
    //doubleValue
    //stringValue
    //pointerValue
    //displayValue


    if (action == 1)
    {
        MethodInfoData data;

        if (!ResolveMethod(
                "Time",
                "set_timeScale",
                data,
                true))
        {
            LOGI("[MYWORK] ResolveMethod FAILED");
            return;
        }

        LOGI("[MYWORK] ResolveMethod SUCCESS");

        hook = new MyHook<m_set_timeScale_t>(data);

        if (hook->Monitor())
        {
            LOGI("[MYWORK] Monitor SUCCESS");
        }
        else
        {
            LOGI("[MYWORK] Monitor FAILED");

            delete hook;
            hook = nullptr;
        }
    }
    else if (action == 0)
    {
        if (!hook)
        {
            LOGI("[MYWORK] Remove: hook is NULL");
            return;
        }

        if (hook->Remove())
        {
            LOGI("[MYWORK] Remove SUCCESS");

            delete hook;
            hook = nullptr;
        }
        else
        {
            LOGI("[MYWORK] Remove FAILED");
        }
    }
}

// -------------------------------------------------


// to replace 1 to active 0 to remove
// int32_t  >> int
// void  >> void
int32_t MyReplacement(
        BetStateController_o* self,
        const MethodInfo* method)
{
    LOGI("[MYWORK] MyReplacement CALLED");

    return 15;
}
void Replace_get_CurrentBet(int action)
{
    using GetCurrentBet_t =
            int32_t (*)(
                    BetStateController_o*,
                    const MethodInfo*
            );

    static MyHook<GetCurrentBet_t>* hook = nullptr;

    if (action == 1)
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
                    (i < data.parameterNames.size() &&
                     data.parameterNames[i])
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

        hook = new MyHook<GetCurrentBet_t>(data);

        if (hook->Replace(MyReplacement))
        {
            LOGI("[MYWORK] Replace SUCCESS");
        }
        else
        {
            LOGI("[MYWORK] Replace FAILED");

            delete hook;
            hook = nullptr;
        }
    }
    else if (action == 0)
    {
        if (!hook)
        {
            LOGI("[MYWORK] Remove: hook is NULL");
            return;
        }

        if (hook->Remove())
        {
            LOGI("[MYWORK] Remove SUCCESS");

            delete hook;
            hook = nullptr;
        }
        else
        {
            LOGI("[MYWORK] Remove FAILED");
        }
    }
}
// ==========================
void MyReplacement_spinSlot(
        SlotMachineManager_o*,
        const MethodInfo*)
{
    LOGI("[MYWORK] MyReplacement CALLED");
}
void Replace_spinSlot(int action)
{
    using Replace_spinSlot_t =
            void (*)(
                    SlotMachineManager_o*,
                    const MethodInfo*
            );

    static MyHook<Replace_spinSlot_t>* hook = nullptr;

    if (action == 1)
    {
        MethodInfoData data;

        if (!ResolveMethod(
                "SlotMachineManager",
                "spinSlot",
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
                    (i < data.parameterNames.size() &&
                     data.parameterNames[i])
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

        hook = new MyHook<Replace_spinSlot_t>(data);

        if (hook->Replace(MyReplacement_spinSlot))
        {
            LOGI("[MYWORK] Replace SUCCESS");
        }
        else
        {
            LOGI("[MYWORK] Replace FAILED");

            delete hook;
            hook = nullptr;
        }
    }
    else if (action == 0)
    {
        if (!hook)
        {
            LOGI("[MYWORK] Remove: hook is NULL");
            return;
        }

        if (hook->Remove())
        {
            LOGI("[MYWORK] Remove SUCCESS");

            delete hook;
            hook = nullptr;
        }
        else
        {
            LOGI("[MYWORK] Remove FAILED");
        }
    }
}
// ==========================

// -------------------------------------------------

// To Test Schedule
void Test_Schedule_SpinSlot()
{
    MethodInfoData data;

    if (!ResolveMethod(
            "SlotMachineManager",
            "spinSlot",
            data,
            true))
    {
        LOGI(
                "[SCHEDULE] spinSlot resolve FAILED"
        );

        return;
    }

    Il2CppObject* instance =
            TestGetInstance(
                    "SlotMachineManager"
            );

    if (!instance)
    {
        LOGI(
                "[SCHEDULE] SlotMachineManager Instance = NULL"
        );

        return;
    }

    auto thisPtr =
            reinterpret_cast<SlotMachineManager_o*>(
                    instance
            );

    const MethodInfo* method =
            data.method;

    PostToMainThread(
            [thisPtr, method]()
            {
                LOGI(
                        "[SCHEDULE] spinSlot CALLBACK TID=%d",
                        gettid()
                );

                using SpinSlot_t =
                        void (*)(
                                SlotMachineManager_o*,
                                const MethodInfo*
                        );

                auto spinSlot =
                        reinterpret_cast<SpinSlot_t>(
                                method->methodPointer
                        );

                spinSlot(
                        thisPtr,
                        nullptr
                );

                LOGI(
                        "[SCHEDULE] spinSlot returned"
                );
            }
    );
}
// to execute method via schedule
void Schedule_spinSlot()
{
    MethodInfoData data;

    if (!ResolveMethod(
            "SlotMachineManager",
            "spinSlot",
            data,
            true))
    {
        LOGI("[MYHOOK] spinSlot resolve FAILED");
        return;
    }

    Il2CppObject* instance =
            TestGetInstance("SlotMachineManager");

    if (!instance)
    {
        LOGI("[MYHOOK] SlotMachineManager Instance = NULL");
        return;
    }

    using SpinSlot_t =
            void (*)(
                    SlotMachineManager_o*,
                    const MethodInfo*
            );

    static MyHook<SpinSlot_t> hook(data);

    auto thisPtr =
            reinterpret_cast<SlotMachineManager_o*>(instance);

    LOGI(
            "[MYHOOK] Calling Schedule | THIS=%p",
            thisPtr
    );

    bool result =
            hook.Schedule(
                    thisPtr,
                    nullptr
            );

    LOGI(
            "[MYHOOK] Schedule returned: %s",
            result ? "TRUE" : "FALSE"
    );
}
void Schedule_SetBetState(int bet)
{
    MethodInfoData data;

    if (!ResolveMethod(
            "SlotMachineManager",
            "SetBetState",
            data,
            true))
    {
        LOGI("[MYHOOK] spinSlot resolve FAILED");
        return;
    }

    Il2CppObject* instance =
            TestGetInstance("SlotMachineManager");

    if (!instance)
    {
        LOGI("[MYHOOK] SlotMachineManager Instance = NULL");
        return;
    }

    using SetBetState_t =
            void (*)(
                    SlotMachineManager_o*,
                    int32_t,
                    const MethodInfo*
            );

    static MyHook<SetBetState_t> hook(data);

    auto thisPtr =
            reinterpret_cast<SlotMachineManager_o*>(instance);

    LOGI(
            "[MYHOOK] Calling Schedule | THIS=%p",
            thisPtr
    );

    bool result = hook.Schedule(thisPtr,bet,nullptr);

    LOGI(
            "[MYHOOK] Schedule returned: %s",
            result ? "TRUE" : "FALSE"
    );
}
// ==========================
// to execute method via schedule (static)
void Schedule_set_timeScale(float speed)
{
    MethodInfoData data;

    if (!ResolveMethod(
            "Time",
            "set_timeScale",
            data,
            true))
    {
        LOGI("[MYHOOK] set_timeScale resolve FAILED");
        return;
    }

    using set_timeScale_t =
            void (*)(
                    float,
                    const MethodInfo*
            );

    static MyHook<set_timeScale_t> hook(data);

    bool result =
            hook.Schedule(
                    speed,
                    nullptr
            );

    LOGI(
            "[MYHOOK] Schedule returned: %s",
            result ? "TRUE" : "FALSE"
    );
}
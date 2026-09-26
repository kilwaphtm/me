#include "hooks.h"
#include <android/log.h>
#include <EGL/egl.h>
#include <dlfcn.h>
#include <pthread.h>
#include <atomic>
#include <unistd.h>
#include "shadowhook.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "il2cpp/il2cpp.h"
#include "il2cpp/resolver.h"
#define LOG_TAG "MY_CUSTOM_SO"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
extern std::atomic<bool> g_ImGuiReady;
extern std::atomic<bool> g_InputHooksInstalled;
extern std::atomic<bool> g_ShadowHookReady;

static void* InputThread(void*);

static EGLBoolean (*g_OriginalEglSwapBuffers)(
        EGLDisplay,
        EGLSurface
) = nullptr;
static void* g_EglStub = nullptr;
static EGLBoolean HookEglSwapBuffers(
        EGLDisplay display,
        EGLSurface surface
)
{
    EGLint width = 0;
    EGLint height = 0;

    eglQuerySurface(
            display,
            surface,
            EGL_WIDTH,
            &width
    );

    eglQuerySurface(
            display,
            surface,
            EGL_HEIGHT,
            &height
    );

    if (!g_ImGuiReady.load())
    {
        if (width > 0 && height > 0)
        {
            InitializeImGui();

            if (g_ImGuiReady.load())
            {
                ImGui::GetIO().DisplaySize =
                        ImVec2(
                                static_cast<float>(width),
                                static_cast<float>(height)
                        );
            }
        }
    }

    if (g_ImGuiReady.load())
    {
        ImGui::GetIO().DisplaySize =
                ImVec2(
                        static_cast<float>(width),
                        static_cast<float>(height)
                );

        ImGui_ImplOpenGL3_NewFrame();

        ImGui::NewFrame();

        RenderImGui();

        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(
                ImGui::GetDrawData()
        );
    }

    return g_OriginalEglSwapBuffers(
            display,
            surface
    );
}
bool InstallEglHook()
{
    void* eglSwapBuffersAddr = dlsym(
            RTLD_DEFAULT,
            "eglSwapBuffers"
    );

    if (!eglSwapBuffersAddr)
    {
        void* eglHandle = dlopen(
                "libEGL.so",
                RTLD_NOW
        );

        if (eglHandle)
        {
            eglSwapBuffersAddr = dlsym(
                    eglHandle,
                    "eglSwapBuffers"
            );
        }
    }

    if (!eglSwapBuffersAddr)
    {
        LOGE("[EGL] eglSwapBuffers NOT FOUND");
        return false;
    }

    g_EglStub = shadowhook_hook_func_addr(
            eglSwapBuffersAddr,
            (void*)HookEglSwapBuffers,
            (void**)&g_OriginalEglSwapBuffers
    );

    if (!g_EglStub || !g_OriginalEglSwapBuffers)
    {
        LOGE("[EGL] eglSwapBuffers HOOK FAILED");
        return false;
    }

    LOGI("[EGL] eglSwapBuffers HOOKED");
    LOGI("[EGL] stub=%p original=%p",
         g_EglStub,
         (void*)g_OriginalEglSwapBuffers);

    return true;
}
static void* HookThread(void*)
{
    LOGI("[HOOK-THREAD] Hook thread started");

    int result = shadowhook_init(
            SHADOWHOOK_MODE_UNIQUE,
            false
    );

    LOGI("[SHADOWHOOK] init result: %d", result);

    if (result != 0)
    {
        LOGE("[SHADOWHOOK] init FAILED");
        return nullptr;
    }

    g_ShadowHookReady.store(true);

    LOGI("[SHADOWHOOK] initialized successfully");

    if (!InstallEglHook())
    {
        LOGE("[HOOK-THREAD] EGL hook FAILED");
    }
    else
    {
        LOGI("[HOOK-THREAD] EGL hook installed");
    }

    pthread_t inputThread;

    const int inputResult = pthread_create(
            &inputThread,
            nullptr,
            InputThread,
            nullptr
    );

    if (inputResult != 0)
    {
        LOGE("[INPUT] pthread_create FAILED: %d", inputResult);
    }
    else
    {
        pthread_detach(inputThread);
        LOGI("[INPUT] Input thread started");
    }

    return nullptr;
}
// =========================================================
// HOOK THREAD
// =========================================================
void StartHookThread()
{
    pthread_t thread;

    const int result = pthread_create(
            &thread,
            nullptr,
            HookThread,
            nullptr
    );

    if (result != 0)
    {
        LOGE(
                "[HOOK-THREAD] pthread_create failed: %d",
                result
        );
        return;
    }

    pthread_detach(thread);

    LOGI("[HOOK-THREAD] Hook thread detached");
}
// =========================================================
// UPDATE IMGUI TOUCH
// =========================================================
void UpdateImGuiTouch()
{
    if (!g_ImGuiReady.load())
        return;

    if (!g_OriginalGetTouchCount)
        return;

    ImGuiIO& io =
            ImGui::GetIO();

    const int count =
            g_OriginalGetTouchCount();

    if (count <= 0)
        return;

    UnityTouch touch{};

    if (!GetUnityTouch(
            0,
            touch
    ))
    {
        return;
    }

    io.AddMouseSourceEvent(
            ImGuiMouseSource_TouchScreen
    );

    float x =
            touch.m_Position.x;

    float y =
            io.DisplaySize.y -
            touch.m_Position.y;

    switch (touch.m_Phase)
    {
        case UnityTouchPhase::Began:

            io.AddMousePosEvent(
                    x,
                    y
            );

            io.AddMouseButtonEvent(
                    0,
                    true
            );

            break;


        case UnityTouchPhase::Moved:

        case UnityTouchPhase::Stationary:

            io.AddMousePosEvent(
                    x,
                    y
            );

            break;


        case UnityTouchPhase::Ended:

        case UnityTouchPhase::Canceled:

            io.AddMousePosEvent(
                    x,
                    y
            );

            io.AddMouseButtonEvent(
                    0,
                    false
            );

            io.AddMousePosEvent(
                    -1,
                    -1
            );

            break;
    }
}
// =========================================================
// get_touchCount HOOK
// =========================================================
int HookGetTouchCount()
{
    if (!g_OriginalGetTouchCount)
        return 0;

    const int count =
            g_OriginalGetTouchCount();

    if (g_ImGuiReady.load())
    {
        UpdateImGuiTouch();

        ImGuiIO& io =
                ImGui::GetIO();

        if (io.WantCaptureMouse)
            return 0;
    }

    return count;
}
// =========================================================
// GetMouseButton HOOK
// =========================================================
bool HookGetMouseButton(
        int button)
{
    if (!g_OriginalGetMouseButton)
        return false;

    const bool result =
            g_OriginalGetMouseButton(
                    button
            );

    if (!g_ImGuiReady.load())
        return result;

    ImGuiIO& io =
            ImGui::GetIO();

    if (io.WantCaptureMouse)
        return false;

    return result;
}
// =========================================================
// INSTALL UNITY INPUT HOOKS
// =========================================================
bool InstallUnityInputHooks()
{
    if (g_InputHooksInstalled.load())
        return true;

    if (
            !g_GetTouchCountMethod ||
            !g_GetMouseButtonMethod
            )
    {
        return false;
    }

    void* touchCountAddress =
            g_GetTouchCountMethod->methodPointer;

    void* mouseButtonAddress =
            g_GetMouseButtonMethod->methodPointer;

    if (!touchCountAddress)
    {
        LOGE(
                "[INPUT] get_touchCount methodPointer NULL"
        );

        return false;
    }

    if (!mouseButtonAddress)
    {
        LOGE(
                "[INPUT] GetMouseButton methodPointer NULL"
        );

        return false;
    }

    LOGI(
            "[INPUT] get_touchCount address = %p",
            touchCountAddress
    );

    LOGI(
            "[INPUT] GetMouseButton address = %p",
            mouseButtonAddress
    );


    // -----------------------------------------------------
    // get_touchCount
    // -----------------------------------------------------

    g_GetTouchCountStub =
            shadowhook_hook_func_addr(
                    touchCountAddress,

                    reinterpret_cast<void*>(
                            HookGetTouchCount
                    ),

                    reinterpret_cast<void**>(
                            &g_OriginalGetTouchCount
                    )
            );

    if (
            !g_GetTouchCountStub ||
            !g_OriginalGetTouchCount
            )
    {
        LOGE(
                "[INPUT] get_touchCount hook FAILED"
        );

        g_GetTouchCountStub = nullptr;
        g_OriginalGetTouchCount = nullptr;

        return false;
    }


    // -----------------------------------------------------
    // GetMouseButton
    // -----------------------------------------------------

    g_GetMouseButtonStub =
            shadowhook_hook_func_addr(
                    mouseButtonAddress,

                    reinterpret_cast<void*>(
                            HookGetMouseButton
                    ),

                    reinterpret_cast<void**>(
                            &g_OriginalGetMouseButton
                    )
            );

    if (
            !g_GetMouseButtonStub ||
            !g_OriginalGetMouseButton
            )
    {
        LOGE(
                "[INPUT] GetMouseButton hook FAILED"
        );

        g_GetMouseButtonStub = nullptr;
        g_OriginalGetMouseButton = nullptr;

        return false;
    }


    g_InputHooksInstalled.store(true);

    LOGI(
            "[INPUT] Unity Input hooks INSTALLED"
    );

    return true;
}
// ========================================================
// INPUT THREAD
// =========================================================
static void* InputThread(void*)
{
    LOGI("[INPUT] Input thread started");

    while (!g_InputHooksInstalled.load())
    {
        if (!g_ShadowHookReady.load())
        {
            sleep(1);
            continue;
        }

        if (!LoadIl2CppAPI())
        {
            LOGI("[INPUT] Waiting for IL2CPP...");
            sleep(1);
            continue;
        }
        if (!FindUnityInputMethods())
        {
            LOGI("[INPUT] Unity Input not ready - retry");
            sleep(1);
            continue;
        }

        // =====================================================
        // TEST METHOD METADATA
        // =====================================================

        const MethodInfo* testMethod =
                FindMethod(
                        "SlotMachineManager",
                        "GetSpinResultSymbolsAnalyticFormat"
                );

        if (testMethod)
        {
            MethodInfoData data;

            if (GetMethodInfoData(testMethod, data))
            {
                LOGI("[TEST] ===== MethodInfoData =====");

                LOGI(
                        "[TEST] Parameter count = %u",
                        data.parameterCount
                );

                LOGI(
                        "[TEST] parameterTypes size = %zu",
                        data.parameterTypes.size()
                );

                LOGI(
                        "[TEST] parameterNames size = %zu",
                        data.parameterNames.size()
                );

                const char* returnType =
                        g_il2cpp_type_get_name(
                                data.returnType
                        );

                LOGI(
                        "[TEST] Return type = %s",
                        returnType ? returnType : "<null>"
                );
            }
            else
            {
                LOGI("[TEST] GetMethodInfoData FAILED");
            }
        }






        if (InstallUnityInputHooks())
        {
            LOGI("[INPUT] Input initialization COMPLETE");
            break;
        }

        LOGI("[INPUT] Hook installation failed - retry");
        sleep(1);
    }

    LOGI("[INPUT] Input thread finished");
    return nullptr;
}

#include "hooks.h"

#include <android/log.h>
#include <EGL/egl.h>
#include <dlfcn.h>

#include "shadowhook.h"
#include <pthread.h>

#define LOG_TAG "MY_CUSTOM_SO"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

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
    return g_OriginalEglSwapBuffers(display, surface);
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

    if (!InstallEglHook())
    {
        LOGE("[HOOK-THREAD] EGL hook FAILED");
    }
    else
    {
        LOGI("[HOOK-THREAD] EGL hook installed");
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
#include <android/log.h>
#include <EGL/egl.h>
#include <GLES3/gl3.h>
#include <dlfcn.h>
#include <pthread.h>
#include <unistd.h>
#include <atomic>
#include <algorithm>
#include <cstdint>
#include <cstring>
#include <cstdio>
#include "shadowhook.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "il2cpp/il2cpp.h"
#include "il2cpp/resolver.h"
#include "hooks/hooks.h"
#include "hooks/mywork.h"
// =========================================================
// LOG
// =========================================================
#define LOG_TAG "MY_CUSTOM_SO"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,  LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
// =========================================================
// EMBEDDED ICON
// =========================================================
extern const unsigned char g_icon_png[];
extern const unsigned int g_icon_png_size;
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// =========================================================
// GLOBAL STATE
// =========================================================
std::atomic<bool> g_ShadowHookReady{false};
std::atomic<bool> g_ImGuiReady{false};
std::atomic<bool> g_InputHooksInstalled{false};
static int g_ScreenWidth = 0;
static int g_ScreenHeight = 0;
static bool LoadEmbeddedIcon();
bool InitializeImGui()
{
    if (g_ImGuiReady.load())
        return true;

    LOGI("[IMGUI] Initializing ImGui...");

    IMGUI_CHECKVERSION();

    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();

    io.DisplaySize = ImVec2(
            static_cast<float>(g_ScreenWidth),
            static_cast<float>(g_ScreenHeight)
    );

    if (!ImGui_ImplOpenGL3_Init("#version 300 es"))
    {
        LOGE("[IMGUI] ImGui_ImplOpenGL3_Init FAILED");

        ImGui::DestroyContext();

        return false;
    }

    LoadEmbeddedIcon();

    g_ImGuiReady.store(true);

    LOGI("[IMGUI] ImGui initialized successfully");

    return true;
}
// =========================================================
// MENU STATE
// =========================================================
namespace MenuState
{
    static bool open = true;

    static int currentPage = 0;
}
// =========================================================
// UI SETTINGS
// =========================================================
namespace UI
{
    // --------------------------------------------------------
    // Base sizes
    // --------------------------------------------------------

    constexpr float BaseFontSize = 26.0f;

    constexpr float MenuMinWidth  = 420.0f;
    constexpr float MenuMinHeight = 420.0f;

    constexpr float MenuMaxWidth = 850.0f;

    constexpr float TabHeight = 58.0f;

    constexpr float ButtonHeight = 58.0f;

    constexpr float FloatingButtonSize = 110.0f;


    // --------------------------------------------------------
    // Colors
    // --------------------------------------------------------

    const ImVec4 Accent =
            ImVec4(
                    0.20f,
                    0.65f,
                    1.00f,
                    1.00f
            );

    const ImVec4 Success =
            ImVec4(
                    0.20f,
                    0.85f,
                    0.45f,
                    1.00f
            );

    const ImVec4 WindowBackground =
            ImVec4(
                    0.035f,
                    0.045f,
                    0.065f,
                    0.98f
            );

    const ImVec4 ChildBackground =
            ImVec4(
                    0.055f,
                    0.070f,
                    0.095f,
                    1.00f
            );

    const ImVec4 FrameBackground =
            ImVec4(
                    0.075f,
                    0.095f,
                    0.125f,
                    1.00f
            );

    const ImVec4 FrameHovered =
            ImVec4(
                    0.10f,
                    0.14f,
                    0.19f,
                    1.00f
            );

    const ImVec4 FrameActive =
            ImVec4(
                    0.12f,
                    0.17f,
                    0.23f,
                    1.00f
            );

    const ImVec4 Button =
            ImVec4(
                    0.08f,
                    0.28f,
                    0.55f,
                    1.00f
            );

    const ImVec4 ButtonHovered =
            ImVec4(
                    0.10f,
                    0.36f,
                    0.68f,
                    1.00f
            );

    const ImVec4 ButtonActive =
            ImVec4(
                    0.07f,
                    0.23f,
                    0.45f,
                    1.00f
            );
}
// =========================================================
// ICON
// =========================================================
static GLuint g_IconTexture = 0;
static int g_IconWidth = 0;
static int g_IconHeight = 0;
// =========================================================
// LOAD EMBEDDED ICON
// =========================================================
static bool LoadEmbeddedIcon()
{
    if (g_IconTexture != 0)
        return true;

    int width = 0;
    int height = 0;
    int channels = 0;

    unsigned char* pixels =
            stbi_load_from_memory(
                    g_icon_png,
                    static_cast<int>(g_icon_png_size),
                    &width,
                    &height,
                    &channels,
                    4
            );

    if (!pixels)
    {
        LOGE("[ICON] Failed to decode icon");
        return false;
    }

    GLuint texture = 0;

    glGenTextures(
            1,
            &texture
    );

    if (texture == 0)
    {
        stbi_image_free(pixels);
        return false;
    }

    glBindTexture(
            GL_TEXTURE_2D,
            texture
    );

    glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MIN_FILTER,
            GL_LINEAR
    );

    glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_MAG_FILTER,
            GL_LINEAR
    );

    glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_S,
            GL_CLAMP_TO_EDGE
    );

    glTexParameteri(
            GL_TEXTURE_2D,
            GL_TEXTURE_WRAP_T,
            GL_CLAMP_TO_EDGE
    );

    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            width,
            height,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            pixels
    );

    glBindTexture(
            GL_TEXTURE_2D,
            0
    );

    stbi_image_free(pixels);

    g_IconTexture = texture;

    g_IconWidth = width;
    g_IconHeight = height;

    LOGI(
            "[ICON] Loaded %dx%d",
            width,
            height
    );

    return true;
}
// =========================================================
// UI STYLE
// =========================================================
static void ApplyUIStyle(float scale)
{
    ImGuiStyle& style = ImGui::GetStyle();


    // --------------------------------------------------------
    // Font
    // --------------------------------------------------------

    style.FontSizeBase =
            UI::BaseFontSize * scale;


    // --------------------------------------------------------
    // Rounding
    // --------------------------------------------------------

    style.WindowRounding =
            20.0f * scale;

    style.ChildRounding =
            16.0f * scale;

    style.FrameRounding =
            12.0f * scale;

    style.PopupRounding =
            10.0f * scale;

    style.ScrollbarRounding =
            10.0f * scale;


    // --------------------------------------------------------
    // Spacing
    // --------------------------------------------------------

    style.WindowPadding =
            ImVec2(
                    22.0f * scale,
                    22.0f * scale
            );

    style.FramePadding =
            ImVec2(
                    14.0f * scale,
                    11.0f * scale
            );

    style.ItemSpacing =
            ImVec2(
                    12.0f * scale,
                    12.0f * scale
            );


    // --------------------------------------------------------
    // Background
    // --------------------------------------------------------

    style.Colors[ImGuiCol_WindowBg] =
            UI::WindowBackground;

    style.Colors[ImGuiCol_ChildBg] =
            UI::ChildBackground;


    // --------------------------------------------------------
    // Frames
    // --------------------------------------------------------

    style.Colors[ImGuiCol_FrameBg] =
            UI::FrameBackground;

    style.Colors[ImGuiCol_FrameBgHovered] =
            UI::FrameHovered;

    style.Colors[ImGuiCol_FrameBgActive] =
            UI::FrameActive;


    // --------------------------------------------------------
    // Buttons
    // --------------------------------------------------------

    style.Colors[ImGuiCol_Button] =
            UI::Button;

    style.Colors[ImGuiCol_ButtonHovered] =
            UI::ButtonHovered;

    style.Colors[ImGuiCol_ButtonActive] =
            UI::ButtonActive;


    // --------------------------------------------------------
    // Header
    // --------------------------------------------------------

    style.Colors[ImGuiCol_Header] =
            UI::Button;

    style.Colors[ImGuiCol_HeaderHovered] =
            UI::ButtonHovered;

    style.Colors[ImGuiCol_HeaderActive] =
            UI::ButtonActive;


    // --------------------------------------------------------
    // Checkbox
    // --------------------------------------------------------

    style.Colors[ImGuiCol_CheckMark] =
            UI::Success;


    // --------------------------------------------------------
    // Slider
    // --------------------------------------------------------

    style.Colors[ImGuiCol_SliderGrab] =
            ImVec4(
                    0.15f,
                    0.55f,
                    0.95f,
                    1.0f
            );

    style.Colors[ImGuiCol_SliderGrabActive] =
            ImVec4(
                    0.20f,
                    0.65f,
                    1.0f,
                    1.0f
            );
}
// =========================================================
// UI COMPONENTS
//
// Add new reusable UI components here.
// =========================================================
namespace Components
{
    // --------------------------------------------------------
    // Button
    // --------------------------------------------------------

    static bool Button(
            const char* label,
            float width,
            float height,
            float scale)
    {
        return ImGui::Button(
                label,
                ImVec2(
                        width * scale,
                        height * scale
                )
        );
    }


    // --------------------------------------------------------
    // Checkbox
    // --------------------------------------------------------

    static bool Checkbox(
            const char* label,
            bool* value)
    {
        return ImGui::Checkbox(
                label,
                value
        );
    }


    // --------------------------------------------------------
    // Slider
    // --------------------------------------------------------

    static bool SliderFloat(
            const char* label,
            float* value,
            float min,
            float max)
    {
        return ImGui::SliderFloat(
                label,
                value,
                min,
                max
        );
    }


    // --------------------------------------------------------
    // Combo
    // --------------------------------------------------------

    static bool Combo(
            const char* label,
            int* current,
            const char* const items[],
            int itemCount)
    {
        return ImGui::Combo(
                label,
                current,
                items,
                itemCount
        );
    }


    // --------------------------------------------------------
    // Text
    // --------------------------------------------------------

    static void Text(
            const char* text)
    {
        ImGui::Text(
                "%s",
                text
        );
    }


    // --------------------------------------------------------
    // Colored Text
    // --------------------------------------------------------

    static void TextColored(
            const ImVec4& color,
            const char* text)
    {
        ImGui::TextColored(
                color,
                "%s",
                text
        );
    }


    // --------------------------------------------------------
    // Section
    // --------------------------------------------------------

    static void Section(
            const char* title)
    {
        ImGui::Spacing();

        ImGui::TextColored(
                UI::Accent,
                "%s",
                title
        );

        ImGui::Separator();

        ImGui::Spacing();
    }


    // --------------------------------------------------------
    // Separator
    // --------------------------------------------------------

    static void Separator()
    {
        ImGui::Separator();
    }


    // --------------------------------------------------------
    // Spacing
    // --------------------------------------------------------

    static void Spacing()
    {
        ImGui::Spacing();
    }
}
// =========================================================
// PAGE SYSTEM
//
// To add a new page:
//
// 1. Create DrawYourPage()
// 2. Add it to pages[]
//
// =========================================================
struct Page
{
    const char* name;

    void (*draw)(float scale);
};
// =========================================================
// EMPTY PAGES
// =========================================================
static void DrawHome(float scale)
{
    if (Components::Button(
            "TEST RESOLVER",
            300.0f,
            UI::ButtonHeight,
            scale
    ))
    {
        TestResolverUsage();
        TestResolverUsage2();
    }
}
static void DrawFeatures(float scale)
{
    (void) scale;

    /*
     * FEATURES PAGE
     *
     * Empty intentionally.
     *
     * Add components here later.
     */
}
static void DrawSettings(float scale)
{
    (void) scale;

    /*
     * SETTINGS PAGE
     *
     * Empty intentionally.
     *
     * Add components here later.
     */
}
// =========================================================
// PAGE LIST
// =========================================================
static Page pages[] =
        {
                {
                        "HOME",
                        DrawHome
                },

                {
                        "FEATURES",
                        DrawFeatures
                },

                {
                        "SETTINGS",
                        DrawSettings
                }
        };
static constexpr int pageCount =
        sizeof(pages) / sizeof(pages[0]);
// =========================================================
// HEADER
// =========================================================
static void DrawHeader(float scale)
{
    ImGui::TextColored(
            UI::Accent,
            "MY MENU"
    );

    ImGui::SameLine();

    ImGui::TextDisabled(
            "  NATIVE"
    );

    const float right =
            ImGui::GetWindowWidth() -
            85.0f * scale;

    ImGui::SameLine(
            right
    );

    ImGui::TextColored(
            UI::Success,
            "ONLINE"
    );

    ImGui::Separator();

    ImGui::Spacing();
}
// =========================================================
// TABS
// =========================================================
static void DrawTabs(float scale)
{
    const float spacing =
            10.0f * scale;

    const float availableWidth =
            ImGui::GetContentRegionAvail().x;

    const float totalSpacing =
            spacing *
            static_cast<float>(
                    pageCount - 1
            );

    const float tabWidth =
            (
                    availableWidth -
                    totalSpacing
            ) /
            static_cast<float>(
                    pageCount
            );


    for (int i = 0; i < pageCount; ++i)
    {
        if (i > 0)
        {
            ImGui::SameLine(
                    0.0f,
                    spacing
            );
        }


        if (Components::Button(
                pages[i].name,
                tabWidth / scale,
                UI::TabHeight,
                scale
        ))
        {
            MenuState::currentPage = i;
        }
    }

    ImGui::Spacing();
}
// =========================================================
// MENU CONTENT
// =========================================================
static void DrawMenuContent(float scale)
{
    DrawHeader(scale);

    DrawTabs(scale);


    // --------------------------------------------------------
    // Current page
    // --------------------------------------------------------

    if (
            MenuState::currentPage >= 0 &&
            MenuState::currentPage < pageCount
            )
    {
        pages[
                MenuState::currentPage
        ].draw(scale);
    }
}
// =========================================================
// FLOATING BUTTON
//
// Appears when menu is closed.
// Can be dragged.
// Clicking opens the menu.
// =========================================================
static void DrawFloatingButton(
        float screenW,
        float screenH,
        float scale)
{
    static ImVec2 buttonPosition(
            20.0f,
            500.0f
    );

    static bool dragging = false;

    static bool moved = false;

    static ImVec2 dragStartMouse;

    static ImVec2 dragStartPosition;


    const float buttonSize =
            UI::FloatingButtonSize * scale;


    // --------------------------------------------------------
    // Keep inside screen
    // --------------------------------------------------------

    buttonPosition.x =
            std::clamp(
                    buttonPosition.x,
                    0.0f,
                    std::max(
                            0.0f,
                            screenW - buttonSize
                    )
            );

    buttonPosition.y =
            std::clamp(
                    buttonPosition.y,
                    0.0f,
                    std::max(
                            0.0f,
                            screenH - buttonSize
                    )
            );


    // --------------------------------------------------------
    // Window
    // --------------------------------------------------------

    ImGui::SetNextWindowPos(
            buttonPosition,
            ImGuiCond_Always
    );

    ImGui::SetNextWindowBgAlpha(
            0.0f
    );


    ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoSavedSettings |
            ImGuiWindowFlags_AlwaysAutoResize;


    ImGui::Begin(
            "##FloatingMenuButton",
            nullptr,
            flags
    );


    // --------------------------------------------------------
    // Transparent button
    // --------------------------------------------------------

    ImGui::PushStyleColor(
            ImGuiCol_Button,
            ImVec4(0, 0, 0, 0)
    );

    ImGui::PushStyleColor(
            ImGuiCol_ButtonHovered,
            ImVec4(0, 0, 0, 0)
    );

    ImGui::PushStyleColor(
            ImGuiCol_ButtonActive,
            ImVec4(0, 0, 0, 0)
    );


    bool clicked = false;


    if (g_IconTexture != 0)
    {
        clicked =
                ImGui::ImageButton(
                        "OpenMenuImage",

                        ImTextureRef(
                                (ImTextureID)(
                                        intptr_t
                                )g_IconTexture
                        ),

                        ImVec2(
                                buttonSize,
                                buttonSize
                        ),

                        ImVec2(0, 0),

                        ImVec2(1, 1),

                        ImVec4(
                                0,
                                0,
                                0,
                                0
                        ),

                        ImVec4(
                                1,
                                1,
                                1,
                                1
                        )
                );
    }
    else
    {
        clicked =
                ImGui::Button(
                        "OPEN",
                        ImVec2(
                                buttonSize,
                                buttonSize
                        )
                );
    }


    ImGui::PopStyleColor(3);


    // --------------------------------------------------------
    // Start drag
    // --------------------------------------------------------

    if (ImGui::IsItemActivated())
    {
        dragging = true;

        moved = false;

        dragStartMouse =
                ImGui::GetIO().MousePos;

        dragStartPosition =
                buttonPosition;
    }


    // --------------------------------------------------------
    // Drag
    // --------------------------------------------------------

    if (dragging)
    {
        ImVec2 currentMouse =
                ImGui::GetIO().MousePos;


        const float dx =
                currentMouse.x -
                dragStartMouse.x;

        const float dy =
                currentMouse.y -
                dragStartMouse.y;


        if (!moved)
        {
            if (
                    dx * dx +
                    dy * dy >
                    100.0f
                    )
            {
                moved = true;
            }
        }


        if (moved)
        {
            buttonPosition.x =
                    dragStartPosition.x + dx;

            buttonPosition.y =
                    dragStartPosition.y + dy;


            buttonPosition.x =
                    std::clamp(
                            buttonPosition.x,
                            0.0f,
                            std::max(
                                    0.0f,
                                    screenW -
                                    buttonSize
                            )
                    );

            buttonPosition.y =
                    std::clamp(
                            buttonPosition.y,
                            0.0f,
                            std::max(
                                    0.0f,
                                    screenH -
                                    buttonSize
                            )
                    );
        }
    }


    // --------------------------------------------------------
    // Release
    // --------------------------------------------------------

    if (ImGui::IsItemDeactivated())
    {
        if (
                !moved &&
                clicked
                )
        {
            MenuState::open = true;
        }


        dragging = false;

        moved = false;
    }


    ImGui::End();
}
// =========================================================
// MENU WINDOW
// =========================================================
static void DrawMainMenu(
        float screenW,
        float screenH,
        float scale)
{
    const float minWidth =
            UI::MenuMinWidth * scale;

    const float minHeight =
            UI::MenuMinHeight * scale;

    const float maxWidth =
            std::min(
                    screenW * 0.95f,
                    UI::MenuMaxWidth * scale
            );

    const float maxHeight =
            screenH * 0.90f;


    const float initialWidth =
            std::min(
                    screenW * 0.88f,
                    UI::MenuMaxWidth * scale
            );

    const float initialHeight =
            std::min(
                    screenH * 0.72f,
                    1050.0f
            );


    // --------------------------------------------------------
    // Initial position
    // --------------------------------------------------------

    ImGui::SetNextWindowPos(
            ImVec2(
                    screenW * 0.5f,
                    screenH * 0.5f
            ),
            ImGuiCond_FirstUseEver,
            ImVec2(
                    0.5f,
                    0.5f
            )
    );


    // --------------------------------------------------------
    // Initial size
    // --------------------------------------------------------

    ImGui::SetNextWindowSize(
            ImVec2(
                    initialWidth,
                    initialHeight
            ),
            ImGuiCond_FirstUseEver
    );


    // --------------------------------------------------------
    // Resize constraints
    // --------------------------------------------------------

    ImGui::SetNextWindowSizeConstraints(
            ImVec2(
                    minWidth,
                    minHeight
            ),

            ImVec2(
                    maxWidth,
                    maxHeight
            )
    );


    ImGuiWindowFlags flags =
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoScrollbar;


    if (ImGui::Begin(
            "My Menu",
            &MenuState::open,
            flags
    ))
    {
        DrawMenuContent(scale);
    }

    ImGui::End();
}
// =========================================================
// DRAW MENU
// =========================================================
void RenderImGui()
{
    if (!g_ImGuiReady.load())
        return;


    ImGuiIO& io =
            ImGui::GetIO();


    const float screenW =
            io.DisplaySize.x;

    const float screenH =
            io.DisplaySize.y;


    if (
            screenW <= 0.0f ||
            screenH <= 0.0f
            )
    {
        return;
    }


    // --------------------------------------------------------
    // Responsive scale
    //
    // 1080 width = 1.0
    // Larger screens get larger UI.
    // --------------------------------------------------------

    const float scale =
            std::clamp(
                    screenW / 1080.0f,
                    1.0f,
                    1.50f
            );


    // --------------------------------------------------------
    // Style
    // --------------------------------------------------------

    ApplyUIStyle(scale);


    // --------------------------------------------------------
    // Menu
    // --------------------------------------------------------

    if (MenuState::open)
    {
        DrawMainMenu(
                screenW,
                screenH,
                scale
        );
    }
    else
    {
        DrawFloatingButton(
                screenW,
                screenH,
                scale
        );
    }
}
// =========================================================
// CONSTRUCTOR
// =========================================================
__attribute__((constructor))
static void Constructor()
{
    LOGI(
            "[CTOR-1] ========== CONSTRUCTOR ENTER =========="
    );

    LOGI(
            "[CTOR-2] Starting HookThread"
    );

    StartHookThread();

    LOGI(
            "[CTOR-5] ========== CONSTRUCTOR EXIT =========="
    );
}

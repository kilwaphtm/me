cmake_minimum_required(VERSION 3.22.1)

project("mymenu")

# =========================================================
# Python
# =========================================================

find_package(Python3 COMPONENTS Interpreter REQUIRED)


# =========================================================
# ShadowHook
# =========================================================

find_package(shadowhook REQUIRED CONFIG)


# =========================================================
# Embed icon.png into C++
# =========================================================

set(ICON_PNG
${CMAKE_CURRENT_SOURCE_DIR}/../icon.png
)

set(GENERATED_ICON_CPP
${CMAKE_CURRENT_BINARY_DIR}/icon_data.cpp
)

add_custom_command(
OUTPUT ${GENERATED_ICON_CPP}

COMMAND ${Python3_EXECUTABLE}
${CMAKE_CURRENT_SOURCE_DIR}/generate_icon.py
${ICON_PNG}
${GENERATED_ICON_CPP}

DEPENDS
${ICON_PNG}
${CMAKE_CURRENT_SOURCE_DIR}/generate_icon.py

COMMENT "Embedding icon.png into libmymenu.so"

VERBATIM
)


# =========================================================
# Native library
# =========================================================

add_library(${CMAKE_PROJECT_NAME} SHARED

        native-lib.cpp
        il2cpp/il2cpp.cpp
        il2cpp/resolver.cpp
        hooks/hooks.cpp

        ${GENERATED_ICON_CPP}

        # Dear ImGui core
        ../imgui/imgui.cpp
        ../imgui/imgui_draw.cpp
        ../imgui/imgui_tables.cpp
        ../imgui/imgui_widgets.cpp

        # Dear ImGui Android backend
        ../imgui/imgui_impl_android.cpp

        # Dear ImGui OpenGL3 backend
        ../imgui/imgui_impl_opengl3.cpp
)



# =========================================================
# Include directories
# =========================================================

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE

../imgui

.
)


# =========================================================
# Libraries
# =========================================================

target_link_libraries(${CMAKE_PROJECT_NAME}

shadowhook::shadowhook

android
log
GLESv3
EGL
dl
)

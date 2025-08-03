# This file tells CMake how to build ImGui into a static library.

find_package(glfw3 3.3 REQUIRED)

add_library(imgui STATIC
    imgui.cpp
    imgui_draw.cpp
    imgui_tables.cpp
    imgui_widgets.cpp
    imgui_demo.cpp

    # --- Backends ---
    backends/imgui_impl_glfw.cpp
    backends/imgui_impl_opengl3.cpp
)

target_include_directories(imgui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/backends
)

# --- ADD THIS LINE ---
# This tells our imgui library about its dependency on glfw.
# It automatically adds glfw's include paths to the imgui build.
target_link_libraries(imgui PRIVATE glfw)
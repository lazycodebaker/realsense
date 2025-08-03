cmake_minimum_required(VERSION 3.16)
project(realsense)

# --- Project-wide Settings ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# --- Output Directories ---
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# --- Compiler Warnings ---
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU|AppleClang")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif(MSVC)
    add_compile_options(/W4)
endif()

# --- Find Dependencies ---
find_package(glfw3 3.3 REQUIRED)
find_package(OpenCV 4 REQUIRED)

# find_package(glfw REQUIRED)
find_package(OpenCV REQUIRED)

# include(FetchContent)

# FetchContent_Declare(
#     imgui
#     GIT_REPOSITORY https://github.com/ocornut/imgui.git
#     GIT_TAG v1.88 # Using a specific tag is good practice
# )
# FetchContent_MakeAvailable(imgui)

# --- Define Third-Party Libraries ---
# Define GLAD as a library so other targets can link to it.
add_library(glad STATIC
    ${CMAKE_SOURCE_DIR}/libs/glad/src/gl.c
)
target_include_directories(glad PUBLIC
    ${CMAKE_SOURCE_DIR}/libs/glad/include
)
# --- Robust OpenCV Wrapper ---
# Create our own interface target with a valid name.
add_library(opencv_wrapper INTERFACE)

# Check if the modern 'OpenCV::OpenCV' target exists.
if(TARGET OpenCV::OpenCV)
    # Modern system: Link our wrapper to the real imported target.
    message(STATUS "Found modern OpenCV::OpenCV target. Linking wrapper.")
    target_link_libraries(opencv_wrapper INTERFACE OpenCV::OpenCV)
else()
    # Legacy system: Populate our wrapper with the old variables.
    message(STATUS "OpenCV::OpenCV target not found. Populating wrapper with legacy variables.")
    target_include_directories(opencv_wrapper INTERFACE ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(opencv_wrapper INTERFACE ${OpenCV_LIBS})
endif()
# From this point on, all our code will use 'opencv_wrapper' for consistency.

# --- Add Your Project Modules ---
add_subdirectory(src/core)
add_subdirectory(src/processing)
add_subdirectory(src/ui)
add_subdirectory(src/app)
add_subdirectory(libs/imgui)

# # --- Installation and Custom Commands ---
# install(DIRECTORY assets/ DESTINATION .)
# install(DIRECTORY config/ DESTINATION .)
# install(DIRECTORY data/ DESTINATION .)

add_custom_target(run
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/realsense
    DEPENDS realsense
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running the realsense application..."
)

# ... (your existing CMakeLists.txt content) ...

add_definitions(-DIMGUI_ENABLE_VIEWPORTS)

# --- Add Your Project Modules ---

# --- Define the Final Executable ---
# This is the missing piece. We define an executable named "realsense"
# using the main.cpp file.
add_executable(realsense
    src/app/main.cpp
)

# Link the final executable against the 'app' library.
# Since 'app' has PUBLIC links to core, processing, and ui,
# this one link will pull in everything we need.
target_link_libraries(realsense PRIVATE
    app
    imgui
    glad
    opencv_wrapper
    glfw
    imgui
)


# --- Installation and Custom Commands ---
install(DIRECTORY assets/ DESTINATION .)
# ... (rest of your file)

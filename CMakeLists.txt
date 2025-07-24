# cmake_minimum_required(VERSION 3.16)

# project(realsense)

# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)

# set(CMAKE_INCLUDE_CURRENT_DIR ON)
# set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# find_package(OpenCV 4 REQUIRED)
# find_package(glfw3 3.3 REQUIRED)

# include(FetchContent)

# set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/libs)

# # FetchContent_Declare(
# #     imgui
# #     GIT_REPOSITORY https://github.com/ocornut/imgui.git
# #     GIT_TAG v1.88
# # )
# # FetchContent_MakeAvailable(imgui)

# # add_library(imgui STATIC
# #     ${imgui_SOURCE_DIR}/imgui.cpp
# #     ${imgui_SOURCE_DIR}/imgui_draw.cpp
# #     ${imgui_SOURCE_DIR}/imgui_tables.cpp
# #     ${imgui_SOURCE_DIR}/imgui_widgets.cpp
# #     ${imgui_SOURCE_DIR}/imgui_demo.cpp
# # )

# # target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

# # FetchContent_Declare(
# #     glad
# #     GIT_REPOSITORY https://github.com/Dav1dde/glad.git
# #     GIT_TAG v0.1.36
# # )
# # FetchContent_MakeAvailable(glad)
 
# # add_library(glad STATIC
# #     ${glad_SOURCE_DIR}/src/glad.c
# # )

# # target_include_directories(glad PUBLIC
# #     ${glad_SOURCE_DIR}/include
# # )

# add_library(glad STATIC
#     ${CMAKE_SOURCE_DIR}/libs/glad/src/gl.c
# )

# target_include_directories(glad PUBLIC
#     ${CMAKE_SOURCE_DIR}/libs/glad/include
# )

# if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU|AppleClang")
#     add_compile_options(-Wall -Wextra -Wpedantic)
# elseif(MSVC)
#     add_compile_options(/W4)
# endif()

# add_subdirectory(src/core)
# add_subdirectory(src/processing)
# add_subdirectory(src/ui)
# add_subdirectory(src/app)

# install(DIRECTORY assets/ DESTINATION ${CMAKE_BINARY_DIR}/assets)
# install(DIRECTORY config/ DESTINATION ${CMAKE_BINARY_DIR}/config)
# install(DIRECTORY data/ DESTINATION ${CMAKE_BINARY_DIR}/data)

# add_executable(realsense src/app/main.cpp)

# target_link_libraries(realsense PRIVATE  glad glfw OpenCV core processing ui)

# add_custom_target(run
#     # COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
#     # COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/config ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config
#     # COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data
#     COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/realsense
#     DEPENDS realsense
#     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#     COMMENT "Running the realsense application with copied assets..."
# )


# cmake_minimum_required(VERSION 3.16)
# project(realsense)

# # --- Project-wide Settings ---
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_EXTENSIONS OFF)
# set(CMAKE_INCLUDE_CURRENT_DIR ON)
# set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# # --- Output Directories ---
# # This ensures binaries (executables) and libraries (.a, .so, .dylib) go to build/bin and build/lib
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
# set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# # --- Compiler Warnings ---
# if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU|AppleClang")
#     add_compile_options(-Wall -Wextra -Wpedantic)
# elseif(MSVC)
#     add_compile_options(/W4)
# endif()

# # --- Find Dependencies ---
# # Use the modern target-based approach for packages
# find_package(OpenCV 4 REQUIRED)
# find_package(glfw3 3.3 REQUIRED)

# # --- Define Third-Party Libraries ---
# # Define GLAD as a library so other targets can link to it.
# # This makes its include directories available to any target that links against it.
# add_library(glad STATIC
#     ${CMAKE_SOURCE_DIR}/libs/glad/src/gl.c
# )
# target_include_directories(glad PUBLIC
#     # PUBLIC makes these include dirs available to targets linking to glad
#     ${CMAKE_SOURCE_DIR}/libs/glad/include
# )

# # --- Add Your Project Modules ---
# # CMake will now process the CMakeLists.txt in each of these directories
# add_subdirectory(src/core)
# add_subdirectory(src/processing)
# add_subdirectory(src/ui)
# add_subdirectory(src/app) # The executable is defined here

# # --- Installation and Custom Commands ---
# # These commands copy assets to the build directory after a build
# install(DIRECTORY assets/ DESTINATION ${CMAKE_BINARY_DIR}/assets)
# install(DIRECTORY config/ DESTINATION ${CMAKE_BINARY_DIR}/config)
# install(DIRECTORY data/ DESTINATION ${CMAKE_BINARY_DIR}/data)

# # This custom target allows you to simply type 'make run' to compile and execute
# add_custom_target(run
#     COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/realsense
#     DEPENDS realsense # Depends on the final executable target defined in src/app
#     WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#     COMMENT "Running the realsense application..."
# )

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

# --- Installation and Custom Commands ---
install(DIRECTORY assets/ DESTINATION . )
install(DIRECTORY config/ DESTINATION . )
install(DIRECTORY data/   DESTINATION . )

add_custom_target(run
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/realsense
    DEPENDS realsense
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running the realsense application..."
)
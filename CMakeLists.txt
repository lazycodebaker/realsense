cmake_minimum_required(VERSION 3.16)

project(realsense)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU|AppleClang")
    add_compile_options(-Wall -Wextra -Wpedantic)
elseif (MSVC)
    add_compile_options(/W4)
endif()

add_subdirectory(libs/imgui)

add_subdirectory(src/core)
add_subdirectory(src/processing)
add_subdirectory(src/ui)
add_subdirectory(src/app)

install(DIRECTORY assets/ DESTINATION ${CMAKE_BINARY_DIR}/assets)
install(DIRECTORY config/ DESTINATION ${CMAKE_BINARY_DIR}/config)
install(DIRECTORY data/ DESTINATION ${CMAKE_BINARY_DIR}/data)

add_executable(realsense src/app/main.cpp)

# Custom "run" target to execute the compiled binary
add_custom_target(run
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/config ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data
    COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/realsense
    DEPENDS realsense
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Running the realsense application with copied assets..."
)

 
# target_link_libraries(realsense PRIVATE core processing ui app)

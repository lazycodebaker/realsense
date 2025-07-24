# Define the final executable
add_executable(realsense
    main.cpp
    Application.cpp
    # Add other .cpp files from src/app/ here
)

# Specify include directories for the 'app' executable
target_include_directories(realsense PRIVATE
    # PRIVATE: only this target needs this include path
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link the executable against all the libraries it uses
target_link_libraries(realsense PRIVATE
    # Link to our custom-built libraries. Because 'ui' has PUBLIC
    # dependencies on glad, glfw, etc., we get them transitively.
    core
    processing
    ui
)
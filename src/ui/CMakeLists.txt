# Define the 'ui' library
add_library(ui
    src/UIManager.cpp
    src/TextureManager.cpp
    # Add other .cpp files from src/ui/src here
)

# Specify include directories for the 'ui' library
target_include_directories(ui PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link 'ui' against its dependencies. This is the key fix!
target_link_libraries(ui PUBLIC
    # It needs GLAD and GLFW for windowing and OpenGL
    glad
    glfw

    # It needs OpenCV for handling cv::Mat. Use the single aggregated target.
    # OpenCV::OpenCV

    # It needs definitions from our other modules
    core
    processing
)

# GLAD needs to be compiled as a library
add_library(glad_lib src/glad.c) # We assume glad.c is generated in the build dir
target_include_directories(glad_lib PUBLIC ${glad_SOURCE_DIR}/include)


add_library(cvt_ui STATIC
    src/UIManager.cpp
    src/TextureManager.cpp
)

target_include_directories(cvt_ui
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(cvt_ui
    PUBLIC
        cvt_core
        cvt_processing
        imgui
        glfw
        glad_lib
        OpenCV::opencv_core
        OpenCV::opencv_imgproc
)

# Platform-specific linking for OpenGL
if(WIN32)
    target_link_libraries(cvt_ui PUBLIC opengl32)
elseif(APPLE)
    target_link_libraries(cvt_ui PUBLIC "-framework OpenGL")
else()
    target_link_libraries(cvt_ui PUBLIC GL)
endif()
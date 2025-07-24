add_library(cvt_core STATIC
    src/CameraManager.cpp
)

target_include_directories(cvt_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(cvt_core
    PUBLIC
        OpenCV::opencv_core
        OpenCV::opencv_videoio
)
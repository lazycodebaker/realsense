add_library(cvt_processing STATIC
    src/VisionEngine.cpp
    src/GrayscaleProcessor.cpp
    src/CannyProcessor.cpp
    src/FaceDetectorProcessor.cpp
)

target_include_directories(cvt_processing
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(cvt_processing
    PUBLIC
        cvt_core
        OpenCV::opencv_core
        OpenCV::opencv_imgproc
        OpenCV::opencv_objdetect
)
add_library(processing
    src/Engine.cpp
    src/CannyProcessor.cpp
    src/FaceDetectorProcessor.cpp
    src/GrayscaleProcessor.cpp
)

target_include_directories(processing PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(processing PUBLIC
opencv_wrapper
core
)

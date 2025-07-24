add_library(processing
    src/Engine.cpp
    src/CannyProcessor.cpp
    src/FaceDetectorProcessor.cpp
    src/GrayscaleProcessor.cpp
)

target_include_directories(processing PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link against core. Since core publicly links to opencv_wrapper,
# 'processing' will get the OpenCV dependency transitively.
target_link_libraries(processing PUBLIC
    core
)
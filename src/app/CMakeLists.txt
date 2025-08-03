
add_library(app
    Application.cpp
)

target_include_directories(app PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link against our reliable wrapper, not directly to OpenCV
target_link_libraries(app PUBLIC
    opencv_wrapper
    core
    processing
    ui
)

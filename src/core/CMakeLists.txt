# # Define the 'core' library
# add_library(core
#     src/CameraManager.cpp
# )

# # Specify this library's own public include directory
# target_include_directories(core PUBLIC
#     ${CMAKE_CURRENT_SOURCE_DIR}/include
# )

# # Link 'core' against its dependencies.
# # This will now work because the top-level CMakeLists.txt ensures
# # that the OpenCV::OpenCV target exists.
# target_link_libraries(core PUBLIC
#     OpenCV::OpenCV
# )

add_library(core
    src/CameraManager.cpp
)

target_include_directories(core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Link against our reliable wrapper, not directly to OpenCV
target_link_libraries(core PUBLIC
    opencv_wrapper
)
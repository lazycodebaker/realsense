# # Define the final executable
# add_executable(realsense
#     main.cpp
#     Application.cpp
#     # Add other .cpp files from src/app/ here
# )

# # Specify include directories for the 'app' executable
# target_include_directories(realsense PRIVATE
#     # PRIVATE: only this target needs this include path
#     ${CMAKE_CURRENT_SOURCE_DIR}/include
# )

# target_include_directories(app PUBLIC
#     ${CMAKE_CURRENT_SOURCE_DIR}/include
# )

# # Link the executable against all the libraries it uses
# target_link_libraries(app PUBLIC
#     # Link to our custom-built libraries. Because 'ui' has PUBLIC
#     # dependencies on glad, glfw, etc., we get them transitively.
#     core
#     processing
#     ui
# )



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

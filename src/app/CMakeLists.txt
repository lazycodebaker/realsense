add_executable(ComputerVisionToolkit
    Application.cpp
    Application.h
    main.cpp
)

target_link_libraries(ComputerVisionToolkit
    PRIVATE
        cvt_core
        cvt_processing
        cvt_ui
)

# Copy data files to the build directory so the executable can find them
file(COPY data/ DESTINATION ${CMAKE_BINARY_DIR})
#pragma once

#include <string>
#include <vector>


struct AppState
{
    float brightness = 0.0f;
    float contrast = 1.0f;
    bool isPaused = false;

    std::string view2_processor_name = "Grayscale";
    std::string view3_processor_name = "Canny";

    std::vector<std::string> available_processors;

    int cannyThreshold1 = 50;
    int cannyThreshold2 = 150;

    double faceScaleFactor = 1.1;
    int faceMinNeighbors = 4;

    float canny_threshold1 = 100.0f;
    float canny_threshold2 = 200.0f;

    float blur_kernel_size = 5.0f;

    // color mask
    float hsv_hue[2] = { 10.f, 30.f };
    float hsv_sat[2] = { 100.f, 255.f };
    float hsv_val[2] = { 100.f, 255.f };

};

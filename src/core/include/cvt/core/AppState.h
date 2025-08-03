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
};

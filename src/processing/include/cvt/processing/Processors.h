#pragma once

#include "cvt/processing/IProcessor.h"
#include <opencv2/opencv.hpp>

// 1. Concrete class for "Original"
class OriginalProcessor : public IProcessor {
public:
    OriginalProcessor(bool visible = false) : IProcessor("Original", visible) {}

    std::string getName() const override { return name; }
    void process(const cv::Mat& inFrame, cv::Mat& outFrame, const AppState& state) override {
        (void)state; // This tells the compiler "I know I'm not using this."
        outFrame = inFrame;
    }
};

// 2. Concrete class for "Grayscale"
class GrayscaleProcessor : public IProcessor {
public:
    GrayscaleProcessor(bool visible = false) : IProcessor("Grayscale", visible) {}

    std::string getName() const override { return name; }

    void process(const cv::Mat& inFrame, cv::Mat& outFrame, const AppState& state) override {
        // Note: The main loop should pass the grayscale frame if needed, or convert here.
        // For simplicity, let's assume the input to every processor is the original RGB frame.
        (void)state;

        cv::Mat gray;
        cv::cvtColor(inFrame, gray, cv::COLOR_RGB2GRAY);
        cv::cvtColor(gray, outFrame, cv::COLOR_GRAY2RGB); // Convert back to 3-channel for display
    }
};

// 3. Concrete class for "Gaussian Blur"
class GaussianBlurProcessor : public IProcessor {
public:
    GaussianBlurProcessor(bool visible = false) : IProcessor("Gaussian Blur", visible) {}

    std::string getName() const override { return name; }

    void process(const cv::Mat& inFrame, cv::Mat& outFrame, const AppState& state) override {
        // We'll need access to blur_kernel_size from AppState
        int k_size = (state.blur_kernel_size / 2) * 2 + 1;
        cv::GaussianBlur(inFrame, outFrame, cv::Size(k_size, k_size), 0);
    }
};

// 4. Concrete class for "Canny Edges"
class CannyProcessor : public IProcessor {
public:
    CannyProcessor(bool visible = false) : IProcessor("Canny Edges", visible) {}

    std::string getName() const override { return name; }

    void process(const cv::Mat& inFrame, cv::Mat& outFrame, const AppState& state) override {
        cv::Mat gray;
        cv::cvtColor(inFrame, gray, cv::COLOR_RGB2GRAY);
        cv::Canny(gray, outFrame, state.canny_threshold1, state.canny_threshold2);
    }
};

// 5. Concrete class for "Contours"
class ContourProcessor : public IProcessor {
public:
    ContourProcessor(bool visible = false) : IProcessor("Contours", visible) {}
    std::string getName() const override { return name; }

    void process(const cv::Mat& inFrame, cv::Mat& outFrame, const AppState& state) override {
        (void)state; 
        cv::Mat gray, canny_output;
        cv::cvtColor(inFrame, gray, cv::COLOR_RGB2GRAY);
        cv::Canny(gray, canny_output, 100, 200);
        std::vector<std::vector<cv::Point>> contours;
        findContours(canny_output, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

        outFrame = cv::Mat::zeros(canny_output.size(), CV_8UC3);
        for (size_t i = 0; i < contours.size(); i++)
        {
            cv::Scalar color = cv::Scalar(rand() % 256, rand() % 256, rand() % 256);
            drawContours(outFrame, contours, (int)i, color, 2);
        }
    }
};

// ... add this to the end of Processors.h

// 6. Concrete class for "Color Mask"
class ColorMaskProcessor : public IProcessor {
public:
    ColorMaskProcessor(bool visible = false) : IProcessor("Color Mask", visible) {}
    std::string getName() const override { return name; }

    void process(const cv::Mat& inFrame, cv::Mat& outFrame, const AppState& state) override {
        cv::Mat hsv;
        cv::cvtColor(inFrame, hsv, cv::COLOR_RGB2HSV);
        cv::inRange(hsv,
            cv::Scalar(state.hsv_hue[0], state.hsv_sat[0], state.hsv_val[0]),
            cv::Scalar(state.hsv_hue[1], state.hsv_sat[1], state.hsv_val[1]),
            outFrame);
    }
};
// ... and so on for every other processor type.

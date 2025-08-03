#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <string>

#include "glad/gl.h"
#include "cvt/ui/TextureManager.h"
#include "cvt/core/AppState.h"

class IProcessor {
public:
    IProcessor(std::string name, bool visible = false);
    virtual ~IProcessor() = default;
    virtual std::string getName() const = 0;
    virtual void process(const cv::Mat& inFrame, cv::Mat& outFrame, const AppState& state) = 0;

    std::string name;
    cv::Mat mat;
    GLuint texture_id = 0;
    bool is_visible = false;

    void initialize_texture(TextureManager& texture_manager);
    void release();
};

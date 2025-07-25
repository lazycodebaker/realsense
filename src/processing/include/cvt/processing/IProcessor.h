#pragma once
#include <opencv2/core.hpp>
#include <string>
#include "cvt/core/AppState.h"
 
class IProcessor {
public:
    virtual ~IProcessor() = default;
    virtual std::string getName() const = 0; 
    virtual void process(const cv::Mat& inFrame, cv::Mat& outFrame, const AppState& state) = 0;
};
#pragma once
#include <opencv2/core.hpp>
#include <string>
#include <memory>
#include <map>
#include "IProcessor.h"

struct ProcessedOutputs
{
    cv::Mat view1_color;
    cv::Mat view2_processed;
    cv::Mat view3_processed;
};

class Engine
{
public:
    void registerProcessor(std::shared_ptr<IProcessor> processor);
    std::vector<std::string> getAvailableProcessors() const;

    ProcessedOutputs run(const cv::Mat &rawFrame, const AppState &state);

private:
    std::map<std::string, std::shared_ptr<IProcessor>> m_processors;
};
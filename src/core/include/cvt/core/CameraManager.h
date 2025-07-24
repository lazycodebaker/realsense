#pragma once

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <thread>
#include <mutex>
#include <atomic>
 
class CameraManager {
public:
    CameraManager(int deviceId = 0);
    ~CameraManager();
 
    bool start();
     void stop();

     bool getLatestFrame(cv::Mat& outFrame);
    bool isRunning() const;

private:
    void captureLoop();

    cv::VideoCapture m_cap;
    std::thread m_thread;
    std::mutex m_mutex;
    std::atomic<bool> m_isRunning;

    cv::Mat m_latestFrame;
    int m_deviceId;
};
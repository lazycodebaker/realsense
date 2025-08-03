
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <thread>
#include <mutex>
#include <atomic>

class CameraManager {
public:
    CameraManager(int frame_width, int frame_height, int deviceId = 0);
    ~CameraManager();

    bool start();
    void stop();
    void open(int deviceId);

    bool getLatestFrame(cv::Mat& outFrame);
    bool isRunning() const;
    bool isOpened() const;
    void capRelease();
    void readFrame(cv::Mat& frame);
    cv::VideoCapture& getCapture() { return m_cap; }

    void updateFrameDimensions(int width, int height);

private:
    void captureLoop();

    cv::VideoCapture m_cap;
    std::thread m_thread;
    std::mutex m_mutex;
    std::atomic<bool> m_isRunning;

    cv::Mat m_latestFrame;
    int m_deviceId;
    int m_frame_width;
    int m_frame_height;
};

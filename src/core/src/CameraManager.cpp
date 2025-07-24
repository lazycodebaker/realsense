#include <cvt/core/CameraManager.h>
#include <iostream>

CameraManager::CameraManager(int deviceId)
    : m_isRunning(false), m_deviceId(deviceId) {}

CameraManager::~CameraManager()
{
    stop();
}

bool CameraManager::start()
{
    m_cap.open(m_deviceId);
    if (!m_cap.isOpened())
    {
        std::cerr << "ERROR: Could not open camera with device ID " << m_deviceId << std::endl;
        return false;
    }
    m_isRunning = true;
    m_thread = std::thread(&CameraManager::captureLoop, this);
    return true;
}

void CameraManager::stop()
{
    m_isRunning = false;
    if (m_thread.joinable())
    {
        m_thread.join();
    }
    if (m_cap.isOpened())
    {
        m_cap.release();
    }
}

bool CameraManager::getLatestFrame(cv::Mat &outFrame)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_latestFrame.empty())
    {
        return false;
    }
    outFrame = m_latestFrame.clone();
    return true;
}

bool CameraManager::isRunning() const
{
    return m_isRunning;
}

void CameraManager::captureLoop()
{
    while (m_isRunning)
    {
        cv::Mat frame;
        if (!m_cap.read(frame))
        {
            std::cerr << "ERROR: Failed to read frame from camera." << std::endl;
            m_isRunning = false;
            break;
        }

        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_latestFrame = frame;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
#pragma once

#include "GLFW/glfw3.h"

class Application
{
public:
    Application();
    void run();

    GLFWwindow *createWindow(int width, int height, const char *title, GLFWmonitor *monitor, GLFWwindow *share);

    const char *getGSLVersion() const
    {
        return this->gsl_version;
    }

private:
    void initialize();
    const char *gsl_version;

    // CameraManager m_cameraManager;
    // Engine m_visionEngine;
    // UIManager m_uiManager;
    // AppState m_appState;
    bool m_isRunning = false;
};

#pragma once

#include <cvt/core/CameraManager.h>
#include <cvt/processing/Engine.h>
#include <cvt/ui/UIManager.h>
#include <cvt/core/AppState.h>

class Application
{
public:
    Application();
    void run();

private:
    void initialize();
    void mainLoop();
    void shutdown();

    CameraManager m_cameraManager;
    Engine m_visionEngine;
    UIManager m_uiManager;
    AppState m_appState;
    bool m_isRunning = false;
};
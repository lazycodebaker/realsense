#include <Application.h>

Application::Application()
{
    m_isRunning = true;
}

void Application::run()
{
    initialize();
    mainLoop();
    shutdown();
}

void Application::mainLoop()
{
    while (m_isRunning)
    {
        // do something here
    }
}

void Application::initialize()
{
    // do something here
}

void Application::shutdown()
{
    // do something here
}
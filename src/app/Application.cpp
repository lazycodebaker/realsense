
#include <iostream>

#include "Application.h"

static void glfw_error_callback(int error, const char *description)
{
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

Application::Application()
{
    m_isRunning = true;
    this->run();
}

void Application::run()
{
    initialize();
}

void Application::initialize()
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return;

    const char *glsl_version = "#version 150";
    this->gsl_version = glsl_version;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
}

GLFWwindow *Application::createWindow(int width, int height, const char *title, GLFWmonitor *monitor, GLFWwindow *share)
{
    GLFWwindow *window = glfwCreateWindow(width, height, title, monitor, share);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable VSync
    return window;
}
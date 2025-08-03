#pragma once

#include <string>
#include <map>
#include <glad/gl.h> 
#include <GLFW/glfw3.h>
#include <opencv2/core.hpp>
#include "cvt/core/AppState.h"
#include "cvt/processing/Engine.h"

class UIManager
{
public:
    UIManager();
    ~UIManager();

    bool init(int width, int height, const std::string &title);
    void shutdown();
    bool windowShouldClose() const;
    void render(AppState &state, const ProcessedOutputs &frames);

private:
    void setupStyle();
    void drawControlPanel(AppState &state);
    void drawStreamViews(const ProcessedOutputs &frames);

    // Texture handling helpers
    void createTexture(GLuint *textureID, int width, int height);
    void updateTexture(GLuint textureID, const cv::Mat &frame);

    GLFWwindow *m_window = nullptr;
    std::map<std::string, GLuint> m_textures;
};
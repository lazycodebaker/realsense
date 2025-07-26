#pragma once
#include <glad/gl.h>
#include <opencv2/core.hpp>

class TextureManager
{
public:
    TextureManager();
    ~TextureManager();

    GLuint createTexture(); // int width, int height);
    void bindTexture();
    void unbindTexture();
    void update(const cv::Mat &frame);

private:
    int m_width = 0;
    int m_height = 0;
};
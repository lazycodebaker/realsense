
#include "cvt/ui/TextureManager.h"

TextureManager::TextureManager() {};

TextureManager::~TextureManager()
{
    delete this;
};

GLuint TextureManager::createTexture() // int width, int height)
{
    // m_width = width;
    // m_height = height;
    // glGenTextures(1, &m_textureID);
    // glBindTexture(GL_TEXTURE_2D, m_textureID);
    // //  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glBindTexture(GL_TEXTURE_2D, 0);

    GLuint texture_id;
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    return texture_id;
}

void TextureManager::update_gl_texture(GLuint texture_id, const cv::Mat &mat, bool is_first_frame)
{
    glBindTexture(GL_TEXTURE_2D, texture_id);

    GLenum input_format = GL_RGB;
    if (mat.channels() == 1)
    {
        input_format = GL_RED;
    }
    else if (mat.channels() == 3)
    {
        input_format = GL_RGB;
    }
    else if (mat.channels() == 4)
    {
        input_format = GL_RGBA;
    }

    // On the first frame, or after a camera switch, we use glTexImage2D to reallocate texture memory.
    if (is_first_frame)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, input_format, mat.cols, mat.rows, 0, input_format, GL_UNSIGNED_BYTE, mat.data);
    }
    else
    {
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mat.cols, mat.rows, input_format, GL_UNSIGNED_BYTE, mat.data);
    }
}
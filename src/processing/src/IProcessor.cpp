
// ARCHITECTURE: A struct to manage each processing stage/feed
#include <utility>
#include "cvt/processing/IProcessor.h"

IProcessor::IProcessor(std::string name, bool visible)
    : name(std::move(name)), texture_id(0), is_visible(visible) {
};

void IProcessor::initialize_texture(TextureManager& texture_manager)
{
    if (texture_id == 0)
    {
        texture_id = texture_manager.createTexture();
    }
}
void IProcessor::release()
{
    if (texture_id != 0)
    {
        glDeleteTextures(1, &texture_id);
        texture_id = 0;
    }
}

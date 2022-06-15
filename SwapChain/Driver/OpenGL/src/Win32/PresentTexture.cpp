#include <GL/Win32/PresentTexture.hpp>

#include <GL/Image/Image.hpp>

#include <GL/glew.h>

namespace OCRA::SwapChain::Win32 {
PresentTexture::PresentTexture(const Image::Handle& a_FromImage)
    : target(a_FromImage->target)
    , dataType(a_FromImage->dataType)
    , dataFormat(a_FromImage->dataFormat)
    , internalFormat(a_FromImage->internalFormat)
    , extent(a_FromImage->info.extent)
{
    glGenSamplers(1, &samplerHandle);
    glGenTextures(1, &handle);
    glBindTexture(target, handle);
    glTexStorage2D(target, 1, internalFormat, extent.width, extent.height);
    glBindTexture(target, 0);
}
PresentTexture::~PresentTexture()
{
    glDeleteSamplers(1, &samplerHandle);
    glDeleteTextures(1, &handle);
}
void PresentTexture::Bind() const
{
    glBindTexture(target, handle);
}

void PresentTexture::Unbind() const
{
    glBindTexture(target, 0);
}
}
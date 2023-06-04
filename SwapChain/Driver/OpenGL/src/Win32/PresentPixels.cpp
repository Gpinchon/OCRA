#include <GL/Win32/PresentPixels.hpp>

#include <GL/glew.h>

namespace OCRA::SwapChain::Win32 {
PresentPixels::PresentPixels(const size_t& a_Size)
    : size(a_Size)
{
    glGenBuffers(1, &handle);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, handle);
    glBufferStorage(GL_PIXEL_UNPACK_BUFFER, size * 3, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
    ptr = glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, size * 3, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}
PresentPixels::~PresentPixels()
{
    glDeleteBuffers(1, &handle);
}
void PresentPixels::Bind()
{
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, handle);
}
void PresentPixels::Unbind()
{
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}
void PresentPixels::Flush()
{
    offset = offsetIndex * size;
    glFlushMappedBufferRange(GL_PIXEL_UNPACK_BUFFER, offset, size);
    ++offsetIndex %= 3;
}
}

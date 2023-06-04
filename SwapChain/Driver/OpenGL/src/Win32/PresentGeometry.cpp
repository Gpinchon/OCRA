#include <GL/Win32/PresentGeometry.hpp>

#include <GL/glew.h>

namespace OCRA::SwapChain::Win32 {
PresentGeometry::PresentGeometry()
{
    glGenBuffers(1, &VBOhandle);
    glBindBuffer(GL_ARRAY_BUFFER, VBOhandle);
    glBufferStorage(GL_ARRAY_BUFFER, 3, nullptr, 0);
    glGenVertexArrays(1, &VAOhandle);
    glBindVertexArray(VAOhandle);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_BYTE, false, 1, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
PresentGeometry::~PresentGeometry()
{
    glDeleteBuffers(1, &VBOhandle);
    glDeleteVertexArrays(1, &VAOhandle);
}
void PresentGeometry::Bind() const
{
    glBindVertexArray(VAOhandle);
}
void PresentGeometry::Unbind() const
{
    glBindVertexArray(0);
}
void PresentGeometry::Draw() const
{
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
}

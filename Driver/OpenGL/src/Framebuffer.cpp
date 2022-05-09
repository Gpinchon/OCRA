#include <FrameBuffer.hpp>

#include <GL/Device.hpp>
#include <GL/FrameBuffer.hpp>
#include <GL/Image/View.hpp>
#include <GL/glew.h>

#include <cassert>
#include "FrameBuffer.hpp"

namespace OCRA::FrameBuffer {
static inline auto CreateFrameBuffer(const Device::Handle& a_Device, const Info& a_Info)
{
    GLuint handle = 0;
    a_Device->PushCommand([a_Info, &handle] {
        glGenFramebuffers(1, &handle);
        glBindFramebuffer(GL_FRAMEBUFFER, handle);
        glFramebufferParameteri(GL_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_WIDTH, a_Info.extent.width);
        glFramebufferParameteri(GL_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_HEIGHT, a_Info.extent.height);
        glFramebufferParameteri(GL_FRAMEBUFFER, GL_FRAMEBUFFER_DEFAULT_LAYERS, a_Info.extent.depth);
        for (uint32_t index = 0; index < a_Info.attachments.size(); ++index) {
            glFramebufferTextureEXT(
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0 + index,
                a_Info.attachments.at(index)->handle,
                0);
        }
        assert(glCheckNamedFramebufferStatusEXT(handle, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }, true);
    return handle;
}
Impl::Impl(const Device::Handle& a_Device, const Info& a_Info)
    : info(a_Info)
    , device(a_Device)
    , handle(CreateFrameBuffer(a_Device, a_Info))
{}
Impl::~Impl() {
    device.lock()->PushCommand([handle = handle] {
        glDeleteFramebuffers(1, &handle);
    }, false);
}
void Impl::Bind()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, handle);
}
void Impl::Unbind()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}
Handle Create(
    const Device::Handle&       a_Device,
    const Info&                 a_Info,
    const AllocationCallback*   a_Allocator)
{
    return Handle(new Impl(a_Device, a_Info));
}
}
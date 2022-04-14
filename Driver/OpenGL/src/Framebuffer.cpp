#include <FrameBuffer.hpp>

#include <cassert>

namespace OCRA::FrameBuffer {
struct Impl {
    Impl(const Device::Handle& a_Device, const Info& a_Info)
        : info(a_Info)
        , device(a_Device)
    {
        a_Device->PushCommand(0, 0, [this] {
            glGenFramebuffers(1, &handle);
            glNamedFramebufferParameteriEXT(handle, GL_FRAMEBUFFER_DEFAULT_WIDTH, info.extent.width);
            glNamedFramebufferParameteriEXT(handle, GL_FRAMEBUFFER_DEFAULT_HEIGHT, info.extent.height);
            glNamedFramebufferParameteriEXT(handle, GL_FRAMEBUFFER_DEFAULT_LAYERS, info.extent.depth);
            for (uint32_t index = 0; index < info.attachments.size(); ++index) {
                glNamedFramebufferTextureEXT(
                    handle,
                    GL_COLOR_ATTACHMENT0 + index,
                    info.attachments.at(index)->handle,
                    0);
            }
            assert(glCheckNamedFramebufferStatusEXT(handle, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        }, true);
    }
    ~Impl() {
        a_Device->PushCommand(0, 0, [handle = handle] {
            glDeleteFramebuffers(1, &handle);
        }, false);
    }
    const Device::WeakHandle device;
    const Info info;
    GLuint handle{ 0 };
};
Handle Create(
    const Device::Handle&       a_Device,
    const Info&                 a_Info,
    const AllocationCallback*   a_Allocator = nullptr)
{
    return Handle(new Impl(a_Device, a_Info));
}
}
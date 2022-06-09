#include <GL/Pipeline/Layout.hpp>
#include <GL/Device.hpp>

#include <GL/glew.h>

#include <algorithm>
#include <cassert>

namespace OCRA::Pipeline::Layout
{
static inline auto PushConstantSize(const Info& a_Info)
{
    uint32_t size = 0;
    for (const auto& range : a_Info.pushConstants)
        size = std::max(size, range.offset + range.size);
    assert(size <= 128);
    return size;
}

static inline auto CreatePushConstantBuffer(const Device::Handle& a_Device, const uint8_t a_Size) {
	GLuint handle = 0;
    if (a_Size > 0) {
        a_Device->PushCommand([&handle, a_Size] {
            glGenBuffers(1, &handle);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, handle);
            glBufferStorage(GL_SHADER_STORAGE_BUFFER, a_Size, nullptr, GL_DYNAMIC_STORAGE_BIT);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        }, true);
    }
	return handle;
}

Impl::Impl(const Device::Handle& a_Device, const Info& a_Info)
    : device(a_Device)
    , info(a_Info)
    , pushConstantSize(PushConstantSize(a_Info))
    , pushConstantHandle(CreatePushConstantBuffer(a_Device, pushConstantSize))
{}

Impl::~Impl()
{
    device.lock()->PushCommand([pushConstantHandle = pushConstantHandle] {
        glDeleteBuffers(1, &pushConstantHandle);
    }, false);
}

Handle Create(const Device::Handle& a_Device, const Info& a_Info, const AllocationCallback* a_Allocator)
{
    return Handle(new Impl(a_Device, a_Info));
}
}
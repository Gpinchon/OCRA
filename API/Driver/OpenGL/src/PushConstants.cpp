#include <GL/Device.hpp>
#include <GL/PushConstants.hpp>

#include <GL/glew.h>

namespace OCRA {
constexpr auto PushConstantMultiBuffering = 3;
int RoundUp(int numToRound, int multiple)
{
    if (multiple == 0)
        return numToRound;

    int remainder = numToRound % multiple;
    if (remainder == 0)
        return numToRound;

    return numToRound + multiple - remainder;
}

static inline auto PushConstantOffsetAlignment(const Device::Handle& a_Device)
{
    GLint offset = 0;
    a_Device->PushCommand([&offset] {
        glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &offset);
    },
        true);
    return offset;
}

static inline auto CreatePushConstantBuffer(const Device::Handle& a_Device, const size_t& a_Size)
{
    const auto size = RoundUp(a_Size, PushConstantOffsetAlignment(a_Device)) * PushConstantMultiBuffering;
    uint32_t handle { 0 };
    if (size > 0) {
        a_Device->PushCommand([&handle, size] {
            glGenBuffers(1, &handle);
            glBindBuffer(GL_UNIFORM_BUFFER, handle);
            glBufferStorage(GL_UNIFORM_BUFFER, size, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
            glBindBuffer(GL_UNIFORM_BUFFER, 0);
        },
            true);
    }
    return handle;
}

static inline auto GetBufferPtr(const Device::Handle& a_Device, const uint32_t& a_Handle, const size_t& a_Size)
{
    void* ptr = nullptr;
    a_Device->PushCommand([&ptr, a_Handle, a_Size] {
        glBindBuffer(GL_UNIFORM_BUFFER, a_Handle);
        ptr = glMapBufferRange(GL_UNIFORM_BUFFER, 0, a_Size, GL_MAP_PERSISTENT_BIT | GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    },
        true);
    return ptr;
}

PushConstants::PushConstants(const Device::Handle& a_Device)
    : device(a_Device)
    , size(256)
    , offsetAlignment(PushConstantOffsetAlignment(a_Device))
    , bufferHandle(CreatePushConstantBuffer(a_Device, size))
    , bufferPtr(GetBufferPtr(a_Device, bufferHandle, size * PushConstantMultiBuffering))
{
}

PushConstants::~PushConstants()
{
    device.lock()->PushCommand([handle = bufferHandle] {
        glDeleteBuffers(1, &handle);
    },
        false);
}

void PushConstants::Bind()
{
    if (size > 0) {
        const auto currentOffset = offset * offsetAlignment;
        glBindBufferRange(GL_UNIFORM_BUFFER, OCRA_GL_SHADER_PUSHCONSTANT_BINDING, bufferHandle, currentOffset, size);
    }
}

void PushConstants::Unbind()
{
    glBindBufferBase(GL_UNIFORM_BUFFER, OCRA_GL_SHADER_PUSHCONSTANT_BINDING, 0);
}

void PushConstants::Update(const size_t& a_Offset, const size_t& a_Size, const std::byte* a_Data)
{
    ++offset %= PushConstantMultiBuffering;
    const auto currentOffset = offset * offsetAlignment;
    std::memcpy((char*)bufferPtr + currentOffset, a_Data, a_Size);
    glFlushMappedNamedBufferRangeEXT(bufferHandle, currentOffset, size);
}
}

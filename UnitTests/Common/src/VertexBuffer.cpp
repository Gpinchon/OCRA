#include <VertexBuffer.hpp>
#include <Common.hpp>

#include <OCRA/OCRA.hpp>

namespace OCRA {
Memory::Handle VertexBuffer::AllocateMemory(const PhysicalDevice::Handle& a_PhysicalDevice, const Device::Handle& a_Device)
{
    return OCRA::AllocateMemory(
        a_PhysicalDevice, a_Device,
        GetSize(),
        MemoryPropertyFlagBits::HostVisible);
}
void VertexBuffer::FillMemory(const void* a_Data)
{
    MemoryMappedRange range;
    range.memory = memory;
    range.length = GetSize();
    range.offset = GetOffset();
    auto bufferPtr = Memory::Map(range);
    std::memcpy(bufferPtr, a_Data, GetSize());
    Memory::Unmap(memory);
}
Buffer::Handle VertexBuffer::CreateBuffer(const Device::Handle& a_Device)
{
    CreateBufferInfo info;
    info.size = GetSize();
    info.usage = BufferUsageFlagBits::VertexBuffer;
    auto buffer = Device::CreateBuffer(a_Device, info);
    Buffer::BindMemory(buffer, memory, GetOffset());
    return buffer;
}
}
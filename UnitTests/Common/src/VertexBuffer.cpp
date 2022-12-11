#include <VertexBuffer.hpp>
#include <Common.hpp>

#include <OCRA/Memory.hpp>
#include <OCRA/Buffer.hpp>

namespace OCRA {
Memory::Handle VertexBuffer::AllocateMemory(const PhysicalDevice::Handle& a_PhysicalDevice, const Device::Handle& a_Device)
{
    return OCRA::AllocateMemory(
        a_PhysicalDevice, a_Device,
        GetSize(),
        PhysicalDevice::MemoryPropertyFlagBits::HostVisible);
}
void VertexBuffer::FillMemory(const Device::Handle& a_Device, const void* a_Data)
{
    Memory::MappedRange range;
    range.memory = memory;
    range.length = GetSize();
    range.offset = GetOffset();
    auto bufferPtr = Memory::Map(a_Device, range);
    std::memcpy(bufferPtr, a_Data, GetSize());
    Memory::Unmap(a_Device, memory);
}
Buffer::Handle VertexBuffer::CreateBuffer(const Device::Handle& a_Device)
{
    Buffer::Info info;
    info.size = GetSize();
    info.usage = Buffer::UsageFlagBits::VertexBuffer;
    auto buffer = Buffer::Create(a_Device, info);
    Buffer::BindMemory(a_Device, buffer, memory, GetOffset());
    return buffer;
}
}
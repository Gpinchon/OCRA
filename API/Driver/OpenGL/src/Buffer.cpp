#include <OCRA/OCRA.hpp>

#include <GL/Buffer.hpp>
#include <GL/Device.hpp>

namespace OCRA::Device
{
Buffer::Handle CreateBuffer(
    const Device::Handle& a_Device,
    const CreateBufferInfo& a_Info,
    const AllocationCallback* a_Allocator)
{
    return std::make_shared<Buffer::Impl>(a_Device, a_Info);
}
Buffer::Handle AllocateBuffer(
    const Handle& a_Device,
    const AllocateBufferInfo& a_Info,
    const AllocationCallback* a_Allocator)
{
    CreateBufferInfo bufferInfo;
    bufferInfo.flags = a_Info.bufferFlags;
    bufferInfo.usage = a_Info.bufferUsage;
    bufferInfo.size  = a_Info.size;
    bufferInfo.sharingMode = a_Info.sharingMode;
    bufferInfo.queueFamilyIndices = a_Info.queueFamilyIndices;
    auto buffer = CreateBuffer(a_Device, bufferInfo, a_Allocator);
    AllocateMemoryInfo memoryInfo;
    memoryInfo.size = a_Info.size;
    memoryInfo.memoryTypeIndex = PhysicalDevice::FindMemoryType(a_Device->physicalDevice.lock(), a_Info.memoryFlags);
    auto memory = AllocateMemory(a_Device, memoryInfo);
    Buffer::BindMemory(buffer, memory, 0);
    return buffer;
}
}

namespace OCRA::Buffer
{
void BindMemory(const Handle& a_Buffer, const Memory::Handle& a_Memory, const size_t& a_MemoryOffset)
{
    a_Buffer->memoryBinding.memory = a_Memory;
    a_Buffer->memoryBinding.offset = a_MemoryOffset;
}
Memory::Handle GetMemory(const Handle& a_Buffer)
{
    return a_Buffer->memoryBinding.memory;
}

size_t GetSizeRequirement(
    const Handle& a_Buffer)
{
    return a_Buffer->info.size;
}
}

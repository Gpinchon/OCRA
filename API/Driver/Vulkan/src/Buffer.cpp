#include <OCRA/OCRA.hpp>

#include <VK/Device.hpp>
#include <VK/Buffer.hpp>
#include <VK/Flags.hpp>
#include <VK/Memory.hpp>
#include <VK/Enums.hpp>
#include <VK/Flags.hpp>

namespace OCRA::Device
{
Buffer::Handle CreateBuffer(
    const Device::Handle& a_Device,
    const CreateBufferInfo& a_Info,
    const AllocationCallback* a_Allocator)
{
    vk::BufferCreateInfo info;
    info.queueFamilyIndexCount = a_Info.queueFamilyIndices.size();
    info.pQueueFamilyIndices = a_Info.queueFamilyIndices.data();
    info.size = a_Info.size;
    info.usage = ConvertToVk(a_Info.usage);
    info.flags = ConvertToVk(a_Info.flags);
    info.sharingMode = ConvertToVk(a_Info.sharingMode);
    return std::make_shared<Buffer::Impl>(*a_Device, info);
}

Buffer::Handle AllocateBuffer(
    const Handle& a_Device,
    const AllocateBufferInfo& a_Info,
    const AllocationCallback* a_Allocator)
{
    auto& device = *a_Device;
    Memory::Handle memory;
    vk::MemoryAllocateInfo  vkMemoryInfo;
    vk::MemoryPropertyFlags vkMemoryFlags = ConvertToVk(a_Info.memoryFlags);
    auto p = device.physicalDevice.getMemoryProperties();
    vkMemoryInfo.allocationSize = a_Info.size;
    for (auto i = 0u; i < p.memoryTypeCount; ++i) {
        const auto& mt = p.memoryTypes[i];
        if ((mt.propertyFlags & vkMemoryFlags)) {
            vkMemoryInfo.memoryTypeIndex = i;
            memory = std::make_shared<Memory::Impl>(device, vkMemoryInfo);
            break;
        }
    }
    CreateBufferInfo bufferInfo;
    bufferInfo.usage = a_Info.bufferUsage;
    bufferInfo.flags = a_Info.bufferFlags;
    bufferInfo.size  = a_Info.size;
    bufferInfo.sharingMode = a_Info.sharingMode;
    bufferInfo.queueFamilyIndices = a_Info.queueFamilyIndices;
    auto buffer = CreateBuffer(a_Device, bufferInfo, a_Allocator);
    BindMemory(buffer, memory, 0);
    return buffer;
}
}

namespace OCRA::Buffer
{
void BindMemory(
    const Handle& a_Buffer,
    const Memory::Handle& a_Memory,
    const size_t& a_MemoryOffset)
{
    auto& buffer = *a_Buffer;
    auto& memory = *a_Memory;
    buffer.memory = a_Memory;
    buffer.bindMemory(*memory, a_MemoryOffset);
}
Memory::Handle GetMemory(const Handle& a_Buffer)
{
    return a_Buffer->memory;
}
}
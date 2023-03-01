#include <VK/Device.hpp>
#include <VK/Buffer.hpp>
#include <VK/Flags.hpp>
#include <VK/Memory.hpp>
#include <VK/Enums.hpp>
#include <VK/Flags.hpp>

#include <OCRA/OCRA.hpp>

namespace OCRA::Device
{
Buffer::Handle CreateBuffer(
    const Device::Handle& a_Device,
    const CreateBufferInfo& a_Info,
    const AllocationCallback* a_Allocator)
{
    VkBuffer buffer = nullptr;
    VkBufferCreateInfo info{ VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
    info.queueFamilyIndexCount = a_Info.queueFamilyIndices.size();
    info.pQueueFamilyIndices = a_Info.queueFamilyIndices.data();
    info.size = a_Info.size;
    info.usage = GetVkBufferUsageFlags(a_Info.usage);
    info.flags = GetVkBufferCreateFlags(a_Info.flags);
    info.sharingMode = GetVkSharingMode(a_Info.sharingMode);
    vkCreateBuffer(*a_Device, &info, nullptr, &buffer);
    return std::make_shared<Buffer::Impl>(*a_Device, buffer);
}

Buffer::Handle AllocateBuffer(
    const Handle& a_Device,
    const AllocateBufferInfo& a_Info,
    const AllocationCallback* a_Allocator)
{
    VkPhysicalDeviceMemoryProperties p;
    VkDeviceMemory        vkMemory = nullptr;
    VkMemoryAllocateInfo  vkMemoryInfo{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
    VkMemoryPropertyFlags vkMemoryFlags = GetVKMemoryPropertyFlags(a_Info.memoryFlags);
    auto& device = *a_Device;
    
    vkGetPhysicalDeviceMemoryProperties(device.physicalDevice, &p);
    vkMemoryInfo.allocationSize = a_Info.size;
    for (auto i = 0u; i < p.memoryTypeCount; ++i) {
        const auto& mt = p.memoryTypes[i];
        if ((mt.propertyFlags & vkMemoryFlags) != 0) {
            vkMemoryInfo.memoryTypeIndex = i;
            vkAllocateMemory(device, &vkMemoryInfo, nullptr, &vkMemory);
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
    auto memory = std::make_shared<Memory::Impl>(device, vkMemory);
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
    buffer.memory = a_Memory;
    vkBindBufferMemory(buffer.device, buffer, *a_Memory, a_MemoryOffset);
}
Memory::Handle GetMemory(const Handle& a_Buffer)
{
    return a_Buffer->memory;
}
}
#include <VK/Device.hpp>
#include <VK/Buffer.hpp>
#include <VK/BufferCreateFlags.hpp>
#include <VK/BufferUsageFlags.hpp>
#include <VK/Memory.hpp>
#include <VK/SharingMode.hpp>

#include <OCRA/Structs.hpp>

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
}

namespace OCRA::Buffer
{
void BindMemory(
    const Handle& a_Buffer,
    const Memory::Handle& a_Memory,
    const size_t& a_MemoryOffset)
{
    const auto& buffer = *a_Buffer;
    vkBindBufferMemory(buffer.device, buffer, *a_Memory, a_MemoryOffset);
}
}
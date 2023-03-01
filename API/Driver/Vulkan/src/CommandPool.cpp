#include <VK/Device.hpp>
#include <VK/CommandPool.hpp>
#include <VK/CommandBuffer.hpp>
#include <VK/Enums.hpp>

#include <OCRA/Structs.hpp>

namespace OCRA::Device
{
Command::Pool::Handle CreateCommandPool(
    const Device::Handle& a_Device,
    const CreateCommandPoolInfo& a_Info,
    const AllocationCallback* a_Allocator)
{
    VkCommandPool pool = nullptr;
    VkCommandPoolCreateInfo info{ VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
    info.queueFamilyIndex = a_Info.queueFamilyIndex;
    info.flags |= (a_Info.flags & CreateCommandPoolFlagBits::Protected) == CreateCommandPoolFlagBits::Protected ? VK_COMMAND_POOL_CREATE_PROTECTED_BIT : 0;
    info.flags |= (a_Info.flags & CreateCommandPoolFlagBits::Transient) == CreateCommandPoolFlagBits::Transient ? VK_COMMAND_POOL_CREATE_TRANSIENT_BIT : 0;
    info.flags |= (a_Info.flags & CreateCommandPoolFlagBits::Reset)     == CreateCommandPoolFlagBits::Reset     ? VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT : 0;
    vkCreateCommandPool(*a_Device, &info, nullptr, &pool);
    return std::make_shared<Command::Pool::Impl>(*a_Device, pool);
}
}

namespace OCRA::Command::Pool
{

std::vector<Buffer::Handle> AllocateCommandBuffer(const AllocateCommandBufferInfo& a_Info)
{
    auto& pool = *a_Info.pool;
    std::vector<Buffer::Handle> buffers;
    std::vector<VkCommandBuffer> vkBuffers;
    vkBuffers.resize(a_Info.count);
    buffers.reserve(vkBuffers.size());
    VkCommandBufferAllocateInfo info{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
    info.level = GetVkCommandBufferLevel(a_Info.level);
    info.commandPool = *a_Info.pool;
    info.commandBufferCount = a_Info.count;
    vkAllocateCommandBuffers(pool.device, &info, vkBuffers.data());
    auto allocator = std::pmr::polymorphic_allocator<Buffer::Impl>(&pool.memoryResource);
    for (const auto& vkBuffer : vkBuffers) {
        auto buffer = std::allocate_shared<Buffer::Impl>(allocator, pool.device, (const VkCommandPool&)pool, vkBuffer);
        buffers.push_back(buffer);
    }
    return buffers;
}
}
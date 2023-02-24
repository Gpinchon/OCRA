#include <VK/Device.hpp>
#include <VK/CommandPool.hpp>
#include <VK/CommandBuffer.hpp>

#include <OCRA/Command/Pool.hpp>

namespace OCRA::Command::Pool
{
Handle Create(
    const Device::Handle& a_Device,
    const Info& a_Info,
    const AllocationCallback* a_Allocator)
{
    VkCommandPool pool = nullptr;
    VkCommandPoolCreateInfo info{ VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
    info.queueFamilyIndex = a_Info.queueFamilyIndex;
    info.flags |= (a_Info.flags & CreateFlagBits::Protected) == CreateFlagBits::Protected ? VK_COMMAND_POOL_CREATE_PROTECTED_BIT : 0;
    info.flags |= (a_Info.flags & CreateFlagBits::Transient) == CreateFlagBits::Transient ? VK_COMMAND_POOL_CREATE_TRANSIENT_BIT : 0;
    info.flags |= (a_Info.flags & CreateFlagBits::Reset) == CreateFlagBits::Reset ? VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT : 0;
    vkCreateCommandPool(*a_Device, &info, nullptr, &pool);
    return std::make_shared<Impl>(*a_Device, pool);
}
std::vector<Buffer::Handle> AllocateBuffer(const AllocateInfo& a_Info)
{
    auto& pool = *a_Info.pool;
    std::vector<Buffer::Handle> buffers;
    std::vector<VkCommandBuffer> vkBuffers;
    buffers.reserve(a_Info.count);
    vkBuffers.reserve(a_Info.count);
    VkCommandBufferAllocateInfo info{};
    vkAllocateCommandBuffers(pool.device, &info, vkBuffers.data());
    auto allocator = std::pmr::polymorphic_allocator<Buffer::Impl>(&pool.memoryResource);
    for (const auto& vkBuffer : vkBuffers) {
        auto buffer = std::allocate_shared<Buffer::Impl>(allocator, pool.device, (const VkCommandPool&)pool, vkBuffer);
        buffers.push_back(buffer);
    }
    return buffers;
}
}
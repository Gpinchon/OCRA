#include <VK/CommandBuffer.hpp>
#include <VK/CommandPool.hpp>
#include <VK/Enums.hpp>

#include <OCRA/Core.hpp>

#include <vector>

namespace OCRA::Command::Pool
{
std::vector<Command::Buffer::Handle> AllocateCommandBuffer(
    const Command::Pool::Handle& a_Pool,
    const AllocateCommandBufferInfo& a_Info)
{
    auto& pool = *a_Pool;
    auto& device = pool.device;
    vk::CommandBufferAllocateInfo info;
    info.level = ConvertToVk(a_Info.level);
    info.commandPool = *pool;
    info.commandBufferCount = a_Info.count;
    auto vkBuffers = device.allocateCommandBuffers(info);
    std::vector<Command::Buffer::Handle> buffers(vkBuffers.size());
    auto allocator = std::pmr::polymorphic_allocator<Command::Buffer::Impl>(&pool.memoryResource);
    for (auto i = 0u ; i < vkBuffers.size(); ++i)
    {
        auto& vkBuffer = vkBuffers.at(i);
        buffers.at(i) = std::allocate_shared<Command::Buffer::Impl>(allocator, std::move(vkBuffer));
    }
    return buffers;
}
}
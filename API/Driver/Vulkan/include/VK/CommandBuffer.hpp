#pragma once

#include <vulkan/vulkan.h>

namespace OCRA::Command::Buffer
{
struct Impl
{
    Impl(const VkDevice& a_Device, const VkCommandPool& a_CommandPool, const VkCommandBuffer& a_CommandBuffer)
        : device(a_Device)
        , commandPool(a_CommandPool)
        , commandBuffer(a_CommandBuffer)
    {}
    ~Impl() {
        vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
    }
    operator auto& () const {
        return commandBuffer;
    }
    const VkDevice        device;
    const VkCommandPool   commandPool;
    const VkCommandBuffer commandBuffer;
};
}
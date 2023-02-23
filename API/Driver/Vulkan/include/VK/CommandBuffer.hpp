#pragma once

#include <vulkan/vulkan.hpp>

namespace OCRA::Command::Buffer {
struct Impl {
    Impl();
    ~Impl() {
        vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
    }
    operator VkCommandBuffer() const {
        return commandBuffer;
    }
    const VkDevice        device;
    const VkCommandPool   commandPool;
    const VkCommandBuffer commandBuffer;
};
}
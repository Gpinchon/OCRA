#pragma once

#include <vulkan/vulkan.hpp>

namespace OCRA::Fence {
struct Impl {
    Impl(const VkDevice& a_Device);
    ~Impl() {
        vkDestroyFence(device, fence, nullptr);
    }
    operator VkFence() const {
        return fence;
    }
    const VkDevice device;
    const VkFence fence;
};
}
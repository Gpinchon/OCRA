#pragma once

#include <vulkan/vulkan.hpp>

namespace OCRA::Fence
{
struct Impl
{
    Impl(const VkDevice& a_Device, const VkFence& a_Fence)
        : device(a_Device)
        , fence(a_Fence)
    {}
    ~Impl() {
        vkDestroyFence(device, fence, nullptr);
    }
    operator auto&() const {
        return fence;
    }
    const VkDevice device;
    const VkFence fence;
};
}
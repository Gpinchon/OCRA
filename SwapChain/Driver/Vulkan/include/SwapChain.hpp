#pragma once

#include <vulkan/vulkan.h>

namespace OCRA::SwapChain
{
struct Impl
{
    Impl(const VkDevice& a_Device, const VkSwapchainKHR& a_SwapChain)
        : device(a_Device)
        , swapChain(a_SwapChain)
    {}
    ~Impl() {
        vkDestroySwapchainKHR(device, swapChain, nullptr);
    }
    operator auto& () const {
        return swapChain;
    }
    const VkDevice       device;
    const VkSwapchainKHR swapChain;

};
}
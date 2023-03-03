#pragma once

#include <OCRA/Handles.hpp>

#include <VK/Image.hpp>

#include <vulkan/vulkan.h>

#include <vector>

namespace OCRA::SwapChain
{
struct ImageStorage {
    alignas(Image::Impl) std::byte storage[sizeof(Image::Impl)];
};
struct Impl
{
    Impl(const VkDevice& a_Device, const VkSwapchainKHR& a_SwapChain)
        : device(a_Device)
        , swapChain(a_SwapChain)
    {}
    ~Impl() {
        vkDestroySwapchainKHR(device, swapChain, nullptr);
    }
    auto& GetCurrentImage() const {
        return images.at(imageIndex);
    }
    operator auto& () const {
        return swapChain;
    }
    const VkDevice       device;
    const VkSwapchainKHR swapChain;
    uint32_t                  imageIndex = 0;
    std::vector<Image::Impl*> images;
    std::vector<ImageStorage> imagesMemory;
};
}
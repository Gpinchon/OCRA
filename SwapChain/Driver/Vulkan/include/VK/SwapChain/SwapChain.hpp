#pragma once

#include <OCRA/Handles.hpp>

#include <VK/Image.hpp>

#include <vulkan/vulkan_raii.hpp>

#include <vector>

namespace OCRA::SwapChain
{
struct ImageStorage {
    alignas(Image::Impl) std::byte storage[sizeof(Image::Impl)];
};
struct Impl : vk::raii::SwapchainKHR
{
    Impl(
        const vk::raii::Device& a_Device,
        const vk::SwapchainCreateInfoKHR& a_Info)
        : vk::raii::SwapchainKHR(a_Device, a_Info)
    {}
    auto& GetCurrentImage() const {
        return images.at(imageIndex);
    }
    uint32_t                   imageIndex = 0;
    std::vector<Image::Handle> images;
    std::vector<ImageStorage>  imagesMemory;
};
}
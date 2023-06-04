#pragma once

#include <VK/Device.hpp>

#include <vulkan/vulkan_raii.hpp>

namespace OCRA::Image {
struct Impl : vk::raii::Image {
    Impl(const Device::Impl& a_Device, const vk::ImageCreateInfo& a_Info)
        : vk::raii::Image(a_Device, a_Info)
        , device(a_Device)
    {
    }
    Impl(const Device::Impl& a_Device, const VkImage& a_Image)
        : vk::raii::Image(a_Device, a_Image)
        , device(a_Device)
    {
    }
    const Device::Impl& device;
    vk::raii::DeviceMemory memory = nullptr;
};
}

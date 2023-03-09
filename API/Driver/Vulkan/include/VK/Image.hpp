#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace OCRA::Image
{
struct Impl : vk::raii::Image
{
    Impl(const vk::raii::Device& a_Device, const vk::ImageCreateInfo& a_Info)
        : vk::raii::Image(a_Device, a_Info)
    {}
    Impl(const vk::raii::Device& a_Device, const VkImage& a_Image)
        : vk::raii::Image(a_Device, a_Image)
    {}
    vk::raii::DeviceMemory memory = nullptr;
};
}
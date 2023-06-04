#pragma once

#include <VK/Device.hpp>

#include <vulkan/vulkan_raii.hpp>

namespace OCRA::Queue {
struct Impl : vk::raii::Queue {
    Impl(
        const vk::raii::Device& a_Device,
        const uint32_t a_Family,
        const uint32_t a_Index)
        : vk::raii::Queue(a_Device, a_Family, a_Index)
    {
    }
};
}

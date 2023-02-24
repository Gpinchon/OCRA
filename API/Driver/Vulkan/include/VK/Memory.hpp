#pragma once

#include <vulkan/vulkan.hpp>

namespace OCRA::Memory
{
struct Impl
{
    Impl(const VkDevice& a_Device, const VkDeviceMemory& a_Memory)
        : device(a_Device)
        , memory(a_Memory)
    {}
    ~Impl() {
        vkFreeMemory(device, memory, nullptr);
    }
    const VkDevice       device;
    const VkDeviceMemory memory;
};
}
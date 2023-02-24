#pragma once

#include <vulkan/vulkan.h>

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
    operator auto& () const {
        return memory;
    }
    const VkDevice       device;
    const VkDeviceMemory memory;
};
}
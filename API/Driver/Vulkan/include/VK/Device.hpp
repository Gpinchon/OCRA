#pragma once

#include <vulkan/vulkan.h>

namespace OCRA::Device
{
struct Impl
{
    Impl(const VkPhysicalDevice& a_PhysicalDevice, const VkDevice& a_Device)
        : physicalDevice(a_PhysicalDevice)
        , device(a_Device)
    {}
    ~Impl() {
        vkDestroyDevice(device, nullptr);
    }
    operator auto& () const {
        return device;
    }
    const VkDevice device;
    const VkPhysicalDevice physicalDevice;
};
}
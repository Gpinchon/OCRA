#pragma once

#include <vulkan/vulkan.hpp>

namespace OCRA::PhysicalDevice
{
//PhysicalDevice::Impl is just a hidden VkPhysicalDevice
struct Impl {
    Impl(const VkPhysicalDevice& a_PhysicalDevice)
        : physicalDevice(a_PhysicalDevice)
    {}
    operator VkPhysicalDevice() {
        return physicalDevice;
    }
    VkPhysicalDevice physicalDevice;
};
}

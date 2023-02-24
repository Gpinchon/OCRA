#pragma once

#include <vulkan/vulkan.hpp>

namespace OCRA::PhysicalDevice
{
struct Impl
{
    Impl(const VkPhysicalDevice& a_PhysicalDevice)
        : physicalDevice(a_PhysicalDevice)
    {}
    operator auto&() {
        return physicalDevice;
    }
    VkPhysicalDevice physicalDevice;
};
}

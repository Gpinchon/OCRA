#pragma once

#include <OCRA/Handle.hpp>
#include <OCRA/PhysicalDevice.hpp>

#include <vulkan/vulkan.hpp>

#include <functional>

OCRA_DECLARE_HANDLE(OCRA::Instance);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Instance);

namespace OCRA::PhysicalDevice
{
struct Impl
{
    Impl(const VkInstance& a_Instance, const VkPhysicalDevice& a_PhysicalDevice);
    const VkInstance        instance;
    const VkPhysicalDevice  physicalDevice;
    const Properties        properties;
    const Features          features;
    const MemoryProperties  memoryProperties;
    const std::vector<QueueFamilyProperties> queueProperties;
};
}

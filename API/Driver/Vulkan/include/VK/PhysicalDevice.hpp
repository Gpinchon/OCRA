#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace OCRA::PhysicalDevice
{
struct Impl : vk::raii::PhysicalDevice
{
    Impl(const vk::raii::Instance& a_Instance, const VkPhysicalDevice& a_PhysicalDevice) noexcept
        : vk::raii::PhysicalDevice(a_Instance, a_PhysicalDevice)
    {}
    inline auto findMemoryType(const vk::MemoryPropertyFlags& a_MemoryProperties) const noexcept
    {
        auto memoryProperties = getMemoryProperties();
        for (auto memoryTypeIndex = 0u; memoryTypeIndex < memoryProperties.memoryTypeCount; ++memoryTypeIndex) {
            if (memoryProperties.memoryTypes[memoryTypeIndex].propertyFlags == a_MemoryProperties)
                return memoryTypeIndex;
        }
        //Couldn't find optimal memory type, take any fitting type
        for (auto memoryTypeIndex = 0u; memoryTypeIndex < memoryProperties.memoryTypeCount; ++memoryTypeIndex) {
            if ((memoryProperties.memoryTypes[memoryTypeIndex].propertyFlags & a_MemoryProperties))
                return memoryTypeIndex;
        }
        return VK_MAX_MEMORY_TYPES;
    }
    inline auto findQueueFamily(const vk::QueueFlags& a_QueueFlags) const
    {
        auto queueProperties = getQueueFamilyProperties();
        for (auto familyIndex = 0u; familyIndex < queueProperties.size(); ++familyIndex) {
            //check if a_QueueFlags is a subset of queueFlags
            if ((queueProperties.at(familyIndex).queueFlags & a_QueueFlags) == a_QueueFlags)
                return familyIndex;
        }
        return std::numeric_limits<uint32_t>::infinity();
    }
};

}

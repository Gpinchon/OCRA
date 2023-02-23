#pragma once

#include <vulkan/vulkan.hpp>

namespace OCRA::Device {
struct Info;
struct Impl {
    Impl(const VkPhysicalDevice& a_PhysicalDevice, const Info& a_Info);
    ~Impl() {
        vkDestroyDevice(device, nullptr);
    }
    operator VkDevice() const {
        return device;
    }
    const VkDevice device;
};
}
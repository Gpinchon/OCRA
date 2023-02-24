#pragma once

#include <vulkan/vulkan.h>

namespace OCRA::Device
{
struct Impl
{
    Impl(const VkDevice& a_Device)
        : device(a_Device)
    {}
    ~Impl() {
        vkDestroyDevice(device, nullptr);
    }
    operator auto& () const {
        return device;
    }
    const VkDevice device;
};
}
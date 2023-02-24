#pragma once

#include <vulkan/vulkan.h>

namespace OCRA::Buffer
{
struct Impl
{
    Impl(const VkDevice& a_Device, const VkBuffer& a_Buffer)
        : device(a_Device)
        , buffer(a_Buffer)
    {}
    ~Impl() {
        vkDestroyBuffer(device, buffer, nullptr);
    }
    operator auto& () const {
        return buffer;
    }
    const VkDevice device;
    const VkBuffer buffer;
};
}
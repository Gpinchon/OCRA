#pragma once

#include <vulkan/vulkan.hpp>

namespace OCRA::FrameBuffer
{
struct Impl
{
    Impl(const VkDevice& a_Device, const VkFramebuffer& a_FB)
        : device(a_Device)
        , framebuffer(a_FB)
    {}
    ~Impl() {
        vkDestroyFramebuffer(device, framebuffer, nullptr);
    }
    operator auto& () const {
        return framebuffer;
    }
    const VkDevice      device;
    const VkFramebuffer framebuffer;
};
}
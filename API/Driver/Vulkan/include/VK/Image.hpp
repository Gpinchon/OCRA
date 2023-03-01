#pragma once

#include <vulkan/vulkan.h>

namespace OCRA::Image
{
struct Impl {
    Impl(const VkDevice& a_Device, const VkImage& a_Image)
        : device(a_Device)
        , image(a_Image)
    {}
    ~Impl() {
        vkDestroyImage(device, image, nullptr);
    }
    operator auto& () const {
        return image;
    }
    const VkDevice device;
    const VkImage  image;
};
}
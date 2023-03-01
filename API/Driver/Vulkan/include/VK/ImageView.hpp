#pragma once

#include <vulkan/vulkan.hpp>

namespace OCRA::Image::View
{
struct Impl
{
    Impl(const VkDevice& a_Device, const VkImageView& a_ImageView)
        : device(a_Device)
        , imageView(a_ImageView)
    {}
    ~Impl() {
        vkDestroyImageView(device, imageView, nullptr);
    }
    operator auto& () const {
        return imageView;
    }
    const VkDevice    device;
    const VkImageView imageView;
};
}
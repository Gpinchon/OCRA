#pragma once

#include <vulkan/vulkan.hpp>

namespace OCRA::Image::View
{
struct Impl
{
    operator auto& () const {
        return sampler;
    }
    VkImageView sampler;
};
}
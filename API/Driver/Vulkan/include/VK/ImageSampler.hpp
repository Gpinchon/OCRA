#pragma once

#include <vulkan/vulkan.hpp>

namespace OCRA::Image::Sampler
{
struct Impl
{
    operator auto& () const {
        return sampler;
    }
    VkSampler sampler;
};
}
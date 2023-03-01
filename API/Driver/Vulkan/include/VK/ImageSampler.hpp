#pragma once

#include <vulkan/vulkan.hpp>

namespace OCRA::Image::Sampler
{
struct Impl
{
    Impl(const VkDevice& a_Device, const VkSampler& a_Sampler)
        : device(a_Device)
        , sampler(a_Sampler)
    {}
    ~Impl() {
        vkDestroySampler(device, sampler, nullptr);
    }
    operator auto& () const {
        return sampler;
    }
    const VkDevice  device;
    const VkSampler sampler;
};
}
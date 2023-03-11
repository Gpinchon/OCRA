#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace OCRA::Image::Sampler
{
struct Impl : vk::raii::Sampler
{
    Impl(
        const vk::raii::Device& a_Device,
        const vk::SamplerCreateInfo& a_Info)
        : vk::raii::Sampler(a_Device, a_Info)
    {}
};
}
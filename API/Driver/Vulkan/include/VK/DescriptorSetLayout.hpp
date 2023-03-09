#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace OCRA::Descriptor::SetLayout
{
struct Impl : vk::raii::DescriptorSetLayout
{
    Impl(const vk::raii::Device& a_Device, const vk::DescriptorSetLayoutCreateInfo& a_Info)
        : vk::raii::DescriptorSetLayout(a_Device, a_Info)
    {}
};
}

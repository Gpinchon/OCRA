#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace OCRA::Descriptor::Set
{
struct Impl : vk::raii::DescriptorSet
{
    Impl(vk::raii::DescriptorSet&& a_DescriptorSet)
        : vk::raii::DescriptorSet(std::move(a_DescriptorSet))
    {}
};
}

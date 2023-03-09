#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace OCRA::Fence
{
struct Impl : vk::raii::Fence
{
    Impl(const vk::raii::Device& a_Device, const vk::FenceCreateInfo& a_Info)
        : vk::raii::Fence(a_Device, a_Info)
    {}
};
}
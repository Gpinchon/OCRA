#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace OCRA::Instance
{
struct Impl : vk::raii::Instance
{
    Impl(vk::raii::Context& a_Context, const vk::InstanceCreateInfo& a_Info)
        : vk::raii::Instance(a_Context, a_Info)
    {}
    //Impl(const VkInstance& a_Instance) noexcept
    //    : vk::raii::Instance(a_Instance)
    //{}
};
}
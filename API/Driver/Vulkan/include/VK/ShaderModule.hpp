#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace OCRA::ShaderModule {
struct Impl : vk::raii::ShaderModule {
    Impl(
        const vk::raii::Device& a_Device,
        const vk::ShaderModuleCreateInfo& a_Info)
        : vk::raii::ShaderModule(a_Device, a_Info)
    {
    }
};
}

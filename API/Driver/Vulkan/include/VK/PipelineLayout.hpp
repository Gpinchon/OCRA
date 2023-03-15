#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace OCRA::Pipeline::Layout
{
struct Impl : vk::raii::PipelineLayout
{
    Impl(
        const vk::raii::Device& a_Device,
        const vk::PipelineLayoutCreateInfo& a_Info)
        : vk::raii::PipelineLayout(a_Device, a_Info)
    {}
};
}
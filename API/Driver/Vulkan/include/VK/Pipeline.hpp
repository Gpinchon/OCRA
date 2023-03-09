#pragma once

#include <OCRA/Enums.hpp>

#include <vulkan/vulkan_raii.hpp>

namespace OCRA::Pipeline
{
struct Impl : vk::raii::Pipeline
{
    Impl(
        const vk::raii::Device& a_Device,
        const vk::GraphicsPipelineCreateInfo& a_Info,
        const vk::PipelineBindPoint& a_BindPoint)
        : vk::raii::Pipeline(a_Device, nullptr, a_Info)
        , bindPoint(a_BindPoint)
    {}
    const vk::PipelineBindPoint bindPoint;
};
}
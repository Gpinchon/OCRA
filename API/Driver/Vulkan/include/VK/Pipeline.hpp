#pragma once

#include <OCRA/Enums.hpp>

#include <vulkan/vulkan_raii.hpp>

namespace OCRA::Pipeline
{
struct Impl : vk::raii::Pipeline
{
    Impl(
        vk::raii::Pipeline&& a_Pipeline,
        const vk::PipelineBindPoint& a_BindPoint)
        : vk::raii::Pipeline(std::move(a_Pipeline))
        , bindPoint(a_BindPoint)
    {}
    const vk::PipelineBindPoint bindPoint;
};
}
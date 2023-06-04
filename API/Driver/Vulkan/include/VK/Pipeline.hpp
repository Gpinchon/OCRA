#pragma once

#include <OCRA/Enums.hpp>

#include <vulkan/vulkan_raii.hpp>

namespace OCRA::Pipeline {
struct Impl : vk::raii::Pipeline {
    Impl(
        vk::raii::Pipeline&& a_Pipeline,
        const vk::PipelineBindPoint& a_BindPoint,
        const vk::PipelineLayout& a_Layout)
        : vk::raii::Pipeline(std::move(a_Pipeline))
        , bindPoint(a_BindPoint)
        , layout(a_Layout)
    {
    }
    const vk::PipelineBindPoint bindPoint;
    const vk::PipelineLayout layout;
};
}

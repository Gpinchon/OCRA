#pragma once

#include <VK/Pipeline.hpp>

#include <OCRA/Enums.hpp>

#include <vulkan/vulkan_raii.hpp>

namespace OCRA::Pipeline::Graphics {
struct Impl : Pipeline::Impl {
    Impl(
        const vk::raii::Device& a_Device,
        const vk::raii::PipelineCache& a_Cache,
        const vk::GraphicsPipelineCreateInfo& a_Info)
        : OCRA::Pipeline::Impl(a_Device.createGraphicsPipeline(a_Cache, a_Info), vk::PipelineBindPoint::eGraphics, a_Info.layout)
    {
    }
};
}

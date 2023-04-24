#pragma once

#include <OCRA/Enums.hpp>

#include <vulkan/vulkan_raii.hpp>

#include <unordered_map>

namespace OCRA::Command::Buffer
{
struct Impl : vk::raii::CommandBuffer
{
    Impl(vk::raii::CommandBuffer&& a_CommandBuffer, const vk::CommandBufferLevel& a_Level)
        : vk::raii::CommandBuffer(std::move(a_CommandBuffer))
        , level(a_Level)
    {}
    auto GetImageLayout(const vk::Image& a_Image) const {
        auto res = imageLayouts.find(a_Image);
        if (res != imageLayouts.end()) return res->second;
        return ImageLayout::Undefined;
    }
    const vk::CommandBufferLevel level;
    std::unordered_map<VkImage, ImageLayout> imageLayouts;
};
}
#pragma once

#include <OCRA/Enums.hpp>

#include <vulkan/vulkan_raii.hpp>

#include <unordered_map>

namespace OCRA::Command::Buffer {
struct Impl : vk::raii::CommandBuffer {
    Impl(vk::raii::CommandBuffer&& a_CommandBuffer,
        const vk::CommandBufferLevel& a_Level)
        : vk::raii::CommandBuffer(std::move(a_CommandBuffer))
        , level(a_Level)
    {}
    const vk::CommandBufferLevel level;
};
}

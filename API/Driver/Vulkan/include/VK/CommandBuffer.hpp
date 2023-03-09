#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace OCRA::Command::Buffer
{
struct Impl : vk::raii::CommandBuffer
{
    Impl(vk::raii::CommandBuffer&& a_CommandBuffer)
        : vk::raii::CommandBuffer(std::move(a_CommandBuffer))
    {}
};
}
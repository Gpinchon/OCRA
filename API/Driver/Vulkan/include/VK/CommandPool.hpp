#pragma once

#include <VK/Device.hpp>

#include <vulkan/vulkan_raii.hpp>

#include <memory_resource>

namespace OCRA::Command::Pool
{
struct Impl : vk::raii::CommandPool
{
    Impl(const Device::Impl& a_Device, const vk::CommandPoolCreateInfo& a_Info)
        : vk::raii::CommandPool(a_Device, a_Info)
        , device(a_Device)
    {}
    const Device::Impl& device;
    std::pmr::unsynchronized_pool_resource memoryResource;
};
}
#pragma once

#include <VK/Device.hpp>

#include <vulkan/vulkan_raii.hpp>

#include <memory_resource>

namespace OCRA::Descriptor::Pool
{
struct Impl : vk::raii::DescriptorPool
{
    Impl(Device::Impl& a_Device, const vk::DescriptorPoolCreateInfo& a_Info)
        : vk::raii::DescriptorPool(a_Device, a_Info)
        , device(a_Device)
    {}
    Device::Impl& device;
    std::pmr::unsynchronized_pool_resource memoryResource;
};
}
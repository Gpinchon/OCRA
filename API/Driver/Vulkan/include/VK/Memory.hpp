#pragma once

#include <VK/Device.hpp>

#include <vulkan/vulkan_raii.hpp>

namespace OCRA::Memory
{
struct Impl : vk::raii::DeviceMemory
{
    Impl(const Device::Impl& a_Device, const vk::MemoryAllocateInfo& a_Info)
        : vk::raii::DeviceMemory(a_Device, a_Info)
        , device(a_Device)
    {}
    const Device::Impl&  device;
};
}
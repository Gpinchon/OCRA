#pragma once

#include <VK/Device.hpp>

#include <OCRA/Handles.hpp>

#include <vulkan/vulkan_raii.hpp>

namespace OCRA::Buffer
{
struct Impl : vk::raii::Buffer
{
    Impl(Device::Impl& a_Device, const vk::BufferCreateInfo& a_Info)
        : vk::raii::Buffer(a_Device, a_Info)
        , device(a_Device)
    {}
    const Device::Impl& device;
    Memory::Handle memory;
};
}
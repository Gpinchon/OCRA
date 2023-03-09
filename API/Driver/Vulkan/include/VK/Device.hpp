#pragma once

#include <VK/PhysicalDevice.hpp>

#include <vulkan/vulkan_raii.hpp>

namespace OCRA::Device
{
struct Impl : vk::raii::Device
{
    Impl(PhysicalDevice::Impl& a_PhysicalDevice, const vk::DeviceCreateInfo& a_Info, const bool a_NeedsTransferQueue)
        : vk::raii::Device(a_PhysicalDevice, a_Info)
        , physicalDevice(a_PhysicalDevice)
    {
        if (!a_NeedsTransferQueue) return; //this device should not be used for transfer
        auto queueFamilyIndex = a_PhysicalDevice.findQueueFamily(vk::QueueFlagBits::eTransfer);
        vk::CommandPoolCreateInfo commandPoolInfo;
        commandPoolInfo.flags = vk::CommandPoolCreateFlagBits::eTransient;
        commandPoolInfo.queueFamilyIndex = queueFamilyIndex;
        transferQueue = getQueue(queueFamilyIndex, 0);
        transferCommandPool = createCommandPool(commandPoolInfo);
    }
    const PhysicalDevice::Impl& physicalDevice;
    vk::raii::Queue          transferQueue{ nullptr };
    vk::raii::CommandPool    transferCommandPool{ nullptr };
};
}
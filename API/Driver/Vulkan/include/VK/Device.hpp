#pragma once

#include <VK/PhysicalDevice.hpp>
#include <VK/PipelineLayoutCache.hpp>
#include <VK/DescriptorSetLayoutCache.hpp>

#include <functional>
#include <unordered_map>

#include <vulkan/vulkan_raii.hpp>

namespace OCRA::Device {
struct Impl : vk::raii::Device {
    using PushDescriptorSetLayoutCache = DescriptorSetLayoutCache<vk::DescriptorSetLayoutCreateFlagBits::ePushDescriptorKHR>;
    using UpdatableDescriptorSetLayoutCache = DescriptorSetLayoutCache<vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool>;
    Impl(PhysicalDevice::Impl& a_PhysicalDevice, const vk::DeviceCreateInfo& a_Info, const bool a_NeedsTransferQueue)
        : vk::raii::Device(a_PhysicalDevice, a_Info)
        , physicalDevice(a_PhysicalDevice)
        , pipelineCache(createPipelineCache({}))
    {
        if (!a_NeedsTransferQueue)
            return; // this device should not be used for transfer
        auto queueFamilyIndex = a_PhysicalDevice.findQueueFamily(vk::QueueFlagBits::eTransfer);
        vk::CommandPoolCreateInfo commandPoolInfo;
        commandPoolInfo.flags            = vk::CommandPoolCreateFlagBits::eTransient;
        commandPoolInfo.queueFamilyIndex = queueFamilyIndex;
        transferQueue                    = getQueue(queueFamilyIndex, 0);
        transferCommandPool              = createCommandPool(commandPoolInfo);
    }
    auto GetOrCreatePushDescriptorSetLayout(
        const std::vector<vk::DescriptorSetLayoutBinding>& a_Bindings)
    {
        return pushDescriptorSetLayoutCache.GetOrCreate(*this, a_Bindings);
    }
    auto GetOrCreateUpdatableDescriptorSetLayout(
        const std::vector<vk::DescriptorSetLayoutBinding>& a_Bindings)
    {
        return updatableDescriptorSetLayoutCache.GetOrCreate(*this, a_Bindings);
    }
    auto GetOrCreatePipelineLayout(
        const vk::DescriptorSetLayout& a_Layout,
        const std::vector<vk::PushConstantRange>& a_PushConstants)
    {
        return pipelineLayoutCache.GetOrCreate(*this, a_Layout, a_PushConstants);
    }
    void ClearCache() {
        pipelineLayoutCache.storage.clear();
        pushDescriptorSetLayoutCache.storage.clear();
        updatableDescriptorSetLayoutCache.storage.clear();
        pipelineCache = createPipelineCache({});
    }
    const PhysicalDevice::Impl& physicalDevice;
    vk::raii::Queue transferQueue { nullptr };
    vk::raii::CommandPool transferCommandPool { nullptr };
    vk::raii::PipelineCache pipelineCache;
    PipelineLayoutCache pipelineLayoutCache;
    PushDescriptorSetLayoutCache pushDescriptorSetLayoutCache;
    UpdatableDescriptorSetLayoutCache updatableDescriptorSetLayoutCache;
};
}

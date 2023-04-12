#pragma once

#include <VK/PhysicalDevice.hpp>

#include <vulkan/vulkan_raii.hpp>

namespace OCRA::Device
{
struct DescriptorSetLayoutCache {
    struct Storage {
        vk::raii::DescriptorSetLayout layout{ nullptr };
        std::vector<vk::DescriptorSetLayoutBinding> bindings;
    };
    auto GetOrCreate(
        const vk::raii::Device& a_Device,
        const std::vector<vk::DescriptorSetLayoutBinding>& a_Bindings) {
        for (auto& layout : layouts) {
            if (layout.bindings.size() != a_Bindings.size()) continue;
            for (auto i = 0u; i < a_Bindings.size(); ++i) {
                auto& binding = layout.bindings.at(i);
                auto& bindingIn = a_Bindings.at(i);
                if (binding != bindingIn) continue;
            }
            return *layout.layout;
        }
        vk::DescriptorSetLayoutCreateInfo info(
            vk::DescriptorSetLayoutCreateFlagBits::ePushDescriptorKHR,
            a_Bindings);
        layouts.resize(layouts.size() + 1);
        auto& storage = layouts.back();
        storage.layout = std::move(a_Device.createDescriptorSetLayout(info));
        storage.bindings = a_Bindings;
        return *storage.layout;
    }
    std::vector<Storage> layouts;
};
struct PipelineLayoutCache {
    struct Storage {
        vk::raii::PipelineLayout layout{ nullptr };
        vk::DescriptorSetLayout  descriptorLayout;
        std::vector<vk::PushConstantRange> pushConstants;
    };
    auto GetOrCreate(
        vk::raii::Device& a_Device,
        const vk::DescriptorSetLayout& a_Layout,
        const std::vector<vk::PushConstantRange>& a_PushConstants)
    {
        for (auto& layout : layouts) {
            if (layout.descriptorLayout != a_Layout) continue;
            if (layout.pushConstants.size() != a_PushConstants.size()) continue;
            for (auto i = 0u; i < a_PushConstants.size(); ++i) {
                auto& pushConstants = layout.pushConstants.at(i);
                auto& pushConstantsIn = a_PushConstants.at(i);
                if (pushConstants != pushConstantsIn) continue;
            }
            return *layout.layout;
        }
        vk::PipelineLayoutCreateInfo info({}, a_Layout, a_PushConstants);
        layouts.resize(layouts.size() + 1);
        auto& storage = layouts.back();
        storage.layout           = std::move(a_Device.createPipelineLayout(info));
        storage.descriptorLayout = a_Layout;
        storage.pushConstants    = a_PushConstants;
        return *storage.layout;
    }
    std::vector<Storage> layouts;
};
struct Impl : vk::raii::Device
{
    Impl(PhysicalDevice::Impl& a_PhysicalDevice, const vk::DeviceCreateInfo& a_Info, const bool a_NeedsTransferQueue)
        : vk::raii::Device(a_PhysicalDevice, a_Info)
        , physicalDevice(a_PhysicalDevice)
        , pipelineCache(createPipelineCache({}))
    {
        if (!a_NeedsTransferQueue) return; //this device should not be used for transfer
        auto queueFamilyIndex = a_PhysicalDevice.findQueueFamily(vk::QueueFlagBits::eTransfer);
        vk::CommandPoolCreateInfo commandPoolInfo;
        commandPoolInfo.flags = vk::CommandPoolCreateFlagBits::eTransient;
        commandPoolInfo.queueFamilyIndex = queueFamilyIndex;
        transferQueue = getQueue(queueFamilyIndex, 0);
        transferCommandPool = createCommandPool(commandPoolInfo);
    }
    auto GetOrCreateDescriptorSetLayout(
        const std::vector<vk::DescriptorSetLayoutBinding>& a_Bindings)
    {
        return descriptorSetLayoutCache.GetOrCreate(*this, a_Bindings);
    }
    auto GetOrCreatePipelineLayout(
        const vk::DescriptorSetLayout& a_Layout,
        const std::vector<vk::PushConstantRange>& a_PushConstants)
    {
        return pipelineLayoutCache.GetOrCreate(*this, a_Layout, a_PushConstants);
    }
    const PhysicalDevice::Impl& physicalDevice;
    vk::raii::Queue          transferQueue{ nullptr };
    vk::raii::CommandPool    transferCommandPool{ nullptr };
    vk::raii::PipelineCache  pipelineCache;
    PipelineLayoutCache      pipelineLayoutCache;
    DescriptorSetLayoutCache descriptorSetLayoutCache;
};
}
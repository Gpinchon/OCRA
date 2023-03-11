#include <VK/CommandBuffer.hpp>
#include <VK/CommandPool.hpp>
#include <VK/DescriptorPool.hpp>
#include <VK/DescriptorSetLayout.hpp>
#include <VK/Device.hpp>
#include <VK/Enums.hpp>
#include <VK/Fence.hpp>
#include <VK/Flags.hpp>
#include <VK/GraphicsPipeline.hpp>
#include <VK/Image.hpp>
#include <VK/ImageView.hpp>
#include <VK/PhysicalDevice.hpp>
#include <VK/Queue.hpp>
#include <VK/Semaphore.hpp>
#include <VK/Structs.hpp>

#include <OCRA/Core.hpp>

namespace OCRA::Device
{
Command::Pool::Handle CreateCommandPool(
    const Device::Handle& a_Device,
    const CreateCommandPoolInfo& a_Info,
    const AllocationCallback* a_Allocator)
{
    vk::CommandPoolCreateInfo info;
    info.queueFamilyIndex = a_Info.queueFamilyIndex;
    info.flags = ConvertToVk(a_Info.flags);
    return std::make_shared<Command::Pool::Impl>(*a_Device, info);
}

Descriptor::Pool::Handle CreateDescriptorPool(const Device::Handle& a_Device, const CreateDescriptorPoolInfo& a_Info, const AllocationCallback* a_Allocator)
{
    vk::DescriptorPoolCreateInfo info;
    std::vector<vk::DescriptorPoolSize> vkPoolSizes;
    vkPoolSizes.reserve(a_Info.sizes.size());
    for (const auto& size : a_Info.sizes) {
        vk::DescriptorPoolSize vkSize{};
        vkSize.descriptorCount = size.count;
        vkSize.type = ConvertToVk(size.type);
        vkPoolSizes.push_back(vkSize);
    }
    info.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet | vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind;
    info.poolSizeCount = vkPoolSizes.size();
    info.pPoolSizes = vkPoolSizes.data();
    info.maxSets = a_Info.maxSets;
    return std::make_shared<Descriptor::Pool::Impl>(*a_Device, info);
}

Descriptor::SetLayout::Handle CreateDescriptorSetLayout(
    const Device::Handle& a_Device,
    const CreateDescriptorSetLayoutInfo& a_Info,
    const AllocationCallback* a_Allocator = nullptr)
{
    auto& device = *a_Device;
    vk::DescriptorSetLayout setLayout = nullptr;
    vk::DescriptorSetLayoutCreateInfo info;
    std::vector<vk::DescriptorSetLayoutBinding> vkBindings;
    vkBindings.reserve(a_Info.bindings.size());
    for (const auto& binding : a_Info.bindings)
    {
        vk::DescriptorSetLayoutBinding vkBinding{};
        vkBinding.binding = binding.binding;
        vkBinding.descriptorCount = binding.count;
        vkBinding.descriptorType = ConvertToVk(binding.type);
        vkBinding.stageFlags = ConvertToVk(binding.stageFlags);
        vkBindings.push_back(vkBinding);
    }
    info.bindingCount = vkBindings.size();
    info.pBindings = vkBindings.data();
    return std::make_shared<Descriptor::SetLayout::Impl>(device, info);
}

Fence::Handle CreateFence(
    const Device::Handle& a_Device,
    const FenceStatus& a_DefaultStatus,
    const AllocationCallback* a_Allocator)
{
    auto& device = *a_Device;
    vk::FenceCreateInfo info;
    info.flags = a_DefaultStatus == FenceStatus::Signaled ? vk::FenceCreateFlagBits::eSignaled : vk::FenceCreateFlags{};
    return std::make_shared<Fence::Impl>(device, info);
}

Pipeline::Handle CreatePipelineGraphics(
    const Device::Handle& a_Device,
    const CreatePipelineGraphicsInfo& a_Info)
{
    auto& device = *a_Device;
    vk::PipelineShaderStageCreateInfo        shaderStageCreateInfo;
    vk::PipelineVertexInputStateCreateInfo   vertexInputStateCreateInfo;
    vk::PipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo;
    vk::PipelineTessellationStateCreateInfo  tessellationStateCreateInfo;
    vk::PipelineViewportStateCreateInfo      viewportStateCreateInfo;
    vk::PipelineRasterizationStateCreateInfo rasterizationStateCreateInfo;
    vk::PipelineMultisampleStateCreateInfo   multisampleStateCreateInfo;
    vk::PipelineDepthStencilStateCreateInfo  depthStencilStateCreateInfo;
    vk::PipelineColorBlendStateCreateInfo    colorBlendStateCreateInfo;
    vk::PipelineDynamicStateCreateInfo       dynamicStateCreateInfo;
    vk::GraphicsPipelineCreateInfo info(
        {}, a_Info.shaderPipelineState.stages.size(),
        &shaderStageCreateInfo,
        &vertexInputStateCreateInfo,
        &inputAssemblyStateCreateInfo,
        &tessellationStateCreateInfo,
        &viewportStateCreateInfo,
        &rasterizationStateCreateInfo,
        &multisampleStateCreateInfo,
        &depthStencilStateCreateInfo,
        &colorBlendStateCreateInfo,
        &dynamicStateCreateInfo);
    return std::make_shared<Pipeline::Graphics::Impl>(device, info);
}

Image::Handle CreateImage(
    const Device::Handle& a_Device,
    const CreateImageInfo& a_Info)
{
    auto& device = *a_Device;
    vk::ImageCreateInfo vkInfo;
    vkInfo.arrayLayers = a_Info.arrayLayers;
    vkInfo.extent = ConvertToVk(a_Info.extent);
    vkInfo.usage = ConvertToVk(a_Info.usage);
    vkInfo.format = ConvertToVk(a_Info.format);
    vkInfo.imageType = ConvertToVk(a_Info.type);
    vkInfo.initialLayout = ConvertToVk(a_Info.initialLayout);
    vkInfo.mipLevels = a_Info.mipLevels;
    vkInfo.samples = ConvertToVk(a_Info.samples);
    auto image = std::make_shared<Image::Impl>(device, vkInfo);
    vk::DeviceImageMemoryRequirements requirements(&vkInfo);
    auto vkMemoryRequirements = device.getImageMemoryRequirements(&vkInfo).memoryRequirements;
    vk::MemoryAllocateInfo vkMemoryInfo;
    vkMemoryInfo.allocationSize = vkMemoryRequirements.size;
    vkMemoryInfo.memoryTypeIndex = device.physicalDevice.findMemoryType(vk::MemoryPropertyFlagBits::eDeviceLocal);
    image->memory = device.allocateMemory(vkMemoryInfo);
    image->bindMemory(*image->memory, 0);
    return image;
}

Image::View::Handle CreateImageView(
    const Device::Handle& a_Device,
    const CreateImageViewInfo& a_Info)
{
    auto& device = *a_Device;
    vk::ImageViewCreateFlags flags;
    vk::ImageViewCreateInfo vkInfo(
        flags,
        **a_Info.image,
        ConvertToVk(a_Info.type),
        ConvertToVk(a_Info.format),
        ConvertToVk(a_Info.components),
        ConvertToVk(a_Info.subRange));
    return std::make_shared<Image::View::Impl>(device, vkInfo, a_Info.image);
}

Semaphore::Handle CreateSemaphore(
    const Device::Handle& a_Device,
    const CreateSemaphoreInfo& a_Info,
    const AllocationCallback* a_Allocator)
{
    vk::SemaphoreCreateInfo info;
    vk::SemaphoreTypeCreateInfo semaphoreType;
    semaphoreType.semaphoreType = a_Info.type == SemaphoreType::Binary ? vk::SemaphoreType::eBinary : vk::SemaphoreType::eTimeline;
    semaphoreType.initialValue = a_Info.initialValue;
    info.pNext = &semaphoreType;
    return std::make_shared<Semaphore::Impl>(*a_Device, info);
}

Queue::Handle GetQueue(
    const Device::Handle& a_Device,
    uint32_t a_FamilyIndex,
    uint32_t a_QueueIndex)
{
    return std::make_shared<Queue::Impl>(*a_Device, a_FamilyIndex, a_QueueIndex);
}
}

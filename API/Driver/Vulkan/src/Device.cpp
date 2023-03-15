#include <VK/CommandBuffer.hpp>
#include <VK/CommandPool.hpp>
#include <VK/DescriptorPool.hpp>
#include <VK/DescriptorSetLayout.hpp>
#include <VK/Device.hpp>
#include <VK/Enums.hpp>
#include <VK/Fence.hpp>
#include <VK/Flags.hpp>
#include <VK/GraphicsPipeline.hpp>
#include <VK/GraphicsPipelineStates.hpp>
#include <VK/Image.hpp>
#include <VK/ImageSampler.hpp>
#include <VK/ImageView.hpp>
#include <VK/PhysicalDevice.hpp>
#include <VK/Queue.hpp>
#include <VK/Semaphore.hpp>
#include <VK/ShaderModule.hpp>
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
    vk::MemoryAllocateInfo vkMemoryInfo;
    vkMemoryInfo.allocationSize = image->getMemoryRequirements().size;
    vkMemoryInfo.memoryTypeIndex = device.physicalDevice.findMemoryType(vk::MemoryPropertyFlagBits::eDeviceLocal);
    image->memory = device.allocateMemory(vkMemoryInfo);
    image->bindMemory(*image->memory, 0);
    return image;
}

Image::Sampler::Handle CreateImageSampler(
    const Device::Handle& a_Device,
    const CreateImageSamplerInfo& a_Info)
{
    vk::SamplerCustomBorderColorCreateInfoEXT borderColor(
        vk::ClearColorValue(
            a_Info.borderColor.r,
            a_Info.borderColor.g,
            a_Info.borderColor.b,
            a_Info.borderColor.a));
    vk::SamplerCreateInfo info({},
        ConvertToVk(a_Info.magFilter),
        ConvertToVk(a_Info.minFilter),
        a_Info.mipmapMode == Filter::Linear ? vk::SamplerMipmapMode::eLinear : vk::SamplerMipmapMode::eNearest,
        ConvertToVk(a_Info.addressModeU),
        ConvertToVk(a_Info.addressModeV), 
        ConvertToVk(a_Info.addressModeW),
        a_Info.mipLodBias,
        a_Info.anisotropyEnable,
        a_Info.maxAnisotropy,
        a_Info.compareEnable,
        ConvertToVk(a_Info.compareOp),
        a_Info.minLod,
        a_Info.maxLod,
        vk::BorderColor::eFloatCustomEXT,
        false, //Because OGL always uses normalized
        &borderColor);
    return std::make_shared<Image::Sampler::Impl>(*a_Device, info);
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

Pipeline::Handle CreatePipelineGraphics(
    const Device::Handle& a_Device,
    const CreatePipelineGraphicsInfo& a_Info)
{
    auto& device = *a_Device;
    
    IntermediateShaderState                     shaderStage(a_Info.shaderPipelineState);
    IntermediateVertexInputState                vertexInputState(a_Info.vertexInputState);
    vk::PipelineInputAssemblyStateCreateInfo    inputAssemblyStateCreateInfo({},
        ConvertToVk(a_Info.inputAssemblyState.topology),
        a_Info.inputAssemblyState.primitiveRestartEnable);
    vk::PipelineTessellationStateCreateInfo     tessellationStateCreateInfo({},
        a_Info.tessellationState.patchControlPoints);
    IntermediateViewportState                   viewportState(a_Info.viewPortState);
    vk::PipelineRasterizationStateCreateInfo    rasterizationStateCreateInfo({},
        a_Info.rasterizationState.depthClampEnable,
        a_Info.rasterizationState.rasterizerDiscardEnable,
        ConvertToVk(a_Info.rasterizationState.polygonMode),
        ConvertToVk(a_Info.rasterizationState.cullMode),
        ConvertToVk(a_Info.rasterizationState.frontFace),
        a_Info.rasterizationState.depthBiasEnable,
        a_Info.rasterizationState.depthBiasConstantFactor,
        a_Info.rasterizationState.depthBiasClamp,
        a_Info.rasterizationState.depthBiasSlopeFactor,
        a_Info.rasterizationState.lineWidth);
    vk::PipelineMultisampleStateCreateInfo      multisampleStateCreateInfo({},
        ConvertToVk(a_Info.multisampleState.rasterizationSamples),
        a_Info.multisampleState.sampleShadingEnable,
        a_Info.multisampleState.minSampleShading,
        a_Info.multisampleState.sampleMasks.data(),
        a_Info.multisampleState.alphaToCoverageEnable,
        a_Info.multisampleState.alphaToOneEnable);
    vk::PipelineDepthStencilStateCreateInfo     depthStencilStateCreateInfo({},
        a_Info.depthStencilState.depthTestEnable,
        a_Info.depthStencilState.depthWriteEnable,
        ConvertToVk(a_Info.depthStencilState.depthCompareOp),
        a_Info.depthStencilState.depthBoundsTestEnable,
        a_Info.depthStencilState.stencilTestEnable,
        ConvertToVk(a_Info.depthStencilState.frontStencilOpState),
        ConvertToVk(a_Info.depthStencilState.backStencilOpState),
        a_Info.depthStencilState.depthBounds.min,
        a_Info.depthStencilState.depthBounds.max);
    IntermediateColorBlendState                 colorBlendState(a_Info.colorBlendState);
    IntermediateDynamicState                    dynamicState(a_Info.dynamicState);

    vk::GraphicsPipelineCreateInfo info({},
        shaderStage.size(), shaderStage.data(),
        vertexInputState.data(),
        &inputAssemblyStateCreateInfo,
        &tessellationStateCreateInfo,
        viewportState.data(),
        &rasterizationStateCreateInfo,
        &multisampleStateCreateInfo,
        &depthStencilStateCreateInfo,
        colorBlendState.data(),
        dynamicState.data());
    return std::make_shared<Pipeline::Graphics::Impl>(device, info);
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

Shader::Module::Handle CreateShaderModule(
    const Device::Handle& a_Device,
    const CreateShaderModuleInfo& a_Info)
{
    return std::make_shared<Shader::Module::Impl>(
        *a_Device, vk::ShaderModuleCreateInfo({}, a_Info.code));
}

Queue::Handle GetQueue(
    const Device::Handle& a_Device,
    uint32_t a_FamilyIndex,
    uint32_t a_QueueIndex)
{
    return std::make_shared<Queue::Impl>(*a_Device, a_FamilyIndex, a_QueueIndex);
}
}

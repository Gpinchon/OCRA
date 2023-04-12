#include <OCRA/OCRA.hpp>

#include <VK/Buffer.hpp>
#include <VK/DescriptorSet.hpp>
#include <VK/PipelineLayout.hpp>
#include <VK/CommandBuffer.hpp>
#include <VK/Enums.hpp>
#include <VK/Flags.hpp>
#include <VK/FrameBuffer.hpp>
#include <VK/Image.hpp>
#include <VK/ImageSampler.hpp>
#include <VK/ImageView.hpp>
#include <VK/Pipeline.hpp>
#include <VK/Structs.hpp>

#include <vulkan/vulkan.h>

namespace OCRA::Command
{
static inline auto GetVkRenderingAttachmentInfo(const RenderingAttachmentInfo& a_Info) {
    vk::RenderingAttachmentInfo vkInfo;
    if (a_Info.imageView == nullptr) return vkInfo;
    vkInfo.clearValue = ConvertToVk(a_Info.clearValue);
    vkInfo.imageLayout = ConvertToVk(a_Info.imageLayout);
    vkInfo.imageView = **a_Info.imageView;
    vkInfo.loadOp = ConvertToVk(a_Info.loadOp);
    //vkInfo.resolveImageLayout
    //vkInfo.resolveImageView
    //vkInfo.resolveImageLayout
    vkInfo.storeOp = ConvertToVk(a_Info.storeOp);
    return vkInfo;
}

void BeginRendering(
    const Command::Buffer::Handle& a_CommandBuffer,
    const RenderingInfo& a_Info)
{
    std::vector<vk::RenderingAttachmentInfo> vkColorAttachments(a_Info.colorAttachments.size());
    vk::RenderingAttachmentInfo vkDepthAttachment;
    vk::RenderingAttachmentInfo vkStencilAttachment;
    for (auto i = 0u; i < vkColorAttachments.size(); ++i) {
        vkColorAttachments.at(i) = GetVkRenderingAttachmentInfo(a_Info.colorAttachments.at(i));
    }
    vkDepthAttachment = GetVkRenderingAttachmentInfo(a_Info.depthAttachment);
    vkStencilAttachment = GetVkRenderingAttachmentInfo(a_Info.stencilAttachment);
    vk::RenderingInfo vkInfo;
    //vkInfo.flags = GetVkRenderingFlags(a_Info.flags);
    //vkInfo.viewMask = a_Info.viewMask;
    vkInfo.flags = {};
    vkInfo.viewMask = 0;
    vkInfo.layerCount = a_Info.layerCount;
    vkInfo.renderArea = ConvertToVk(a_Info.area);
    vkInfo.setColorAttachments(vkColorAttachments);
    vkInfo.pDepthAttachment = &vkDepthAttachment;
    vkInfo.pStencilAttachment = &vkStencilAttachment;
    a_CommandBuffer->beginRendering(vkInfo);
}

//void BeginRenderPass(
//    const Command::Buffer::Handle& a_CommandBuffer,
//    const RenderPassBeginInfo& a_BeginInfo,
//    const SubPassContents& a_SubPassContents)
//{
//    std::vector<vk::ClearValue> vkClearValues(a_BeginInfo.colorClearValues.size() + 1);
//    for (auto i = 0u; i < a_BeginInfo.colorClearValues.size(); ++i)
//    {
//        auto& clearValue = a_BeginInfo.colorClearValues.at(i);
//        auto& vkClearValue = vkClearValues.at(i);
//        vkClearValue.color = ConvertToVk(clearValue);
//    }
//    vkClearValues.back().depthStencil = ConvertToVk(a_BeginInfo.depthStencilClearValue);
//    vk::RenderPassBeginInfo vkInfo;
//    vkInfo.clearValueCount = vkClearValues.size();
//    vkInfo.framebuffer = *a_BeginInfo.framebuffer;
//    vkInfo.pClearValues = vkClearValues.data();
//}

void BindPipeline(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Pipeline::Handle& a_Pipeline)
{
    auto& pipeline = *a_Pipeline;
    a_CommandBuffer->bindPipeline(vk::PipelineBindPoint(pipeline.bindPoint), *pipeline);
}

void BindVertexBuffers(
    const Command::Buffer::Handle& a_CommandBuffer,
    const uint32_t                              a_FirstBinding,
    const std::vector<OCRA::Buffer::Handle>& a_VertexBuffers,
    const std::vector<uint64_t>& a_Offsets)
{
    std::vector<vk::Buffer> buffers(a_VertexBuffers.size());
    std::transform(a_VertexBuffers.begin(), a_VertexBuffers.end(),
        buffers.begin(), [](const auto& buffer) {
            return **buffer;
        });
    a_CommandBuffer->bindVertexBuffers(a_FirstBinding, buffers, a_Offsets);
}

void ClearColorImage(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Image::Handle& a_Image,
    const ColorValue& a_Color,
    const std::vector<ImageSubresourceRange>& a_Ranges)
{
    static_assert(sizeof(ColorValue) == sizeof(vk::ClearColorValue));
    std::vector<vk::ImageSubresourceRange> vkRanges(a_Ranges.size());
    for (auto i = 0u; i < a_Ranges.size(); ++i)
        vkRanges.at(i) = ConvertToVk(a_Ranges.at(i));
    a_CommandBuffer->clearColorImage(**a_Image,
        vk::ImageLayout::eTransferDstOptimal,
        reinterpret_cast<const vk::ClearColorValue&>(a_Color),
        vkRanges);
}

void CopyBuffer(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Buffer::Handle& a_SrcBuffer,
    const OCRA::Buffer::Handle& a_DstBuffer,
    const std::vector<BufferCopyRegion>& a_Regions)
{
    auto& srcBuffer = *a_SrcBuffer;
    auto& dstBuffer = *a_DstBuffer;
    std::vector<vk::BufferCopy> vkRegions;
    vkRegions.reserve(a_Regions.size());
    for (const auto& region : a_Regions) {
        VkBufferCopy vkRegion{};
        vkRegion.dstOffset = region.writeOffset;
        vkRegion.srcOffset = region.readOffset;
        vkRegion.size      = region.size;
        vkRegions.push_back(vkRegion);
    }
    a_CommandBuffer->copyBuffer(*srcBuffer, *dstBuffer, vkRegions);
}

void CopyImage(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Image::Handle& a_SrcImage,
    const Image::Handle& a_DstImage,
    const std::vector<ImageCopy>& a_Regions)
{
    std::vector<vk::ImageCopy> vkCopies(a_Regions.size());
    ImageAspectFlags srcImageAspects;
    ImageAspectFlags dstImageAspects;
    for (auto i = 0u; i < vkCopies.size(); ++i)
    {
        auto& copy = a_Regions.at(i);
        auto& vkCopy = vkCopies.at(i);
        vkCopy.extent         = ConvertToVk(copy.extent);
        vkCopy.dstOffset      = ConvertToVk(copy.dstOffset);
        vkCopy.dstSubresource = ConvertToVk(copy.dstSubresource);
        vkCopy.srcOffset      = ConvertToVk(copy.srcOffset);
        vkCopy.srcSubresource = ConvertToVk(copy.srcSubresource);
        srcImageAspects |= copy.srcSubresource.aspects;
        dstImageAspects |= copy.dstSubresource.aspects;
    }
    auto& srcImageLayout = a_CommandBuffer->imageLayouts[**a_SrcImage];
    if (srcImageLayout != ImageLayout::TransferSrcOptimal) {
        ImageLayoutTransitionInfo transition;
        transition.image = a_SrcImage;
        transition.subRange.aspects = srcImageAspects;
        transition.oldLayout = srcImageLayout;
        transition.newLayout = ImageLayout::TransferSrcOptimal;
        TransitionImageLayout(a_CommandBuffer, transition);
    }
    auto& dstImageLayout = a_CommandBuffer->imageLayouts[**a_DstImage];
    if (dstImageLayout != ImageLayout::TransferDstOptimal) {
        ImageLayoutTransitionInfo transition;
        transition.image = a_DstImage;
        transition.subRange.aspects = dstImageAspects;
        transition.oldLayout = dstImageLayout;
        transition.newLayout = ImageLayout::TransferDstOptimal;
        TransitionImageLayout(a_CommandBuffer, transition);
    }
    a_CommandBuffer->copyImage(
        **a_SrcImage, vk::ImageLayout::eTransferSrcOptimal,
        **a_DstImage, vk::ImageLayout::eTransferDstOptimal,
        vkCopies);
}

void Draw(
    const Command::Buffer::Handle& a_CommandBuffer,
    const uint32_t a_VertexCount,
    const uint32_t a_InstanceCount,
    const uint32_t a_FirstVertex,
    const uint32_t a_FirstInstance)
{
    a_CommandBuffer->draw(
        a_VertexCount, a_InstanceCount,
        a_FirstVertex, a_FirstInstance);
}

void EndRendering(const Command::Buffer::Handle& a_CommandBuffer)
{
    a_CommandBuffer->endRendering();
}

void ExecuteCommandBuffer(
    const Buffer::Handle& a_CommandBuffer,
    const Buffer::Handle& a_SecondaryCommandBuffer)
{
    a_CommandBuffer->executeCommands({ **a_SecondaryCommandBuffer });
}

void PipelineBarrier(
    const Buffer::Handle& a_CommandBuffer,
    const PipelineStageFlags& a_SrcStageMask,
    const PipelineStageFlags& a_DstStageMask,
    const DependencyFlags& a_DependencyFlags,
    const std::vector<MemoryBarrier>& a_MemoryBarriers,
    const std::vector<BufferMemoryBarrier>& a_BufferMemoryBarriers,
    const std::vector<ImageMemoryBarrier>& a_ImageMemoryBarriers)
{
    std::vector<vk::MemoryBarrier>       vkMemoryBarriers;
    std::vector<vk::BufferMemoryBarrier> vkBufferMemoryBarriers;
    std::vector<vk::ImageMemoryBarrier>  vkImageMemoryBarriers;
    vkMemoryBarriers.reserve(a_MemoryBarriers.size());
    for (const auto& barrier : a_MemoryBarriers) {
        vk::MemoryBarrier vkBarrier;
        vkBarrier.dstAccessMask = ConvertToVk(barrier.dstAccessMask);
        vkBarrier.srcAccessMask = ConvertToVk(barrier.srcAccessMask);
        vkMemoryBarriers.push_back(vkBarrier);
    }
    vkBufferMemoryBarriers.reserve(a_BufferMemoryBarriers.size());
    for (const auto& barrier : a_BufferMemoryBarriers) {
        vk::BufferMemoryBarrier vkBarrier;
        vkBarrier.dstAccessMask = ConvertToVk(barrier.dstAccessMask);
        vkBarrier.srcAccessMask = ConvertToVk(barrier.srcAccessMask);
        vkBarrier.dstQueueFamilyIndex = barrier.dstQueueFamilyIndex;
        vkBarrier.srcQueueFamilyIndex = barrier.srcQueueFamilyIndex;
        vkBarrier.buffer = **barrier.buffer;
        vkBarrier.offset = barrier.offset;
        vkBarrier.size = barrier.size;
        vkBufferMemoryBarriers.push_back(vkBarrier);
    }
    vkImageMemoryBarriers.reserve(a_ImageMemoryBarriers.size());
    for (const auto& barrier : a_ImageMemoryBarriers) {
        vk::ImageMemoryBarrier vkBarrier;
        vkBarrier.image = **barrier.image;
        vkBarrier.oldLayout = ConvertToVk(barrier.oldLayout);
        vkBarrier.newLayout = ConvertToVk(barrier.newLayout);
        vkBarrier.dstAccessMask = ConvertToVk(barrier.dstAccessMask);
        vkBarrier.srcAccessMask = ConvertToVk(barrier.srcAccessMask);
        vkBarrier.dstQueueFamilyIndex = barrier.dstQueueFamilyIndex;
        vkBarrier.srcQueueFamilyIndex = barrier.srcQueueFamilyIndex;
        vkBarrier.subresourceRange.aspectMask = ConvertToVk(barrier.subRange.aspects);
        vkBarrier.subresourceRange.baseArrayLayer = barrier.subRange.baseArrayLayer;
        vkBarrier.subresourceRange.baseMipLevel = barrier.subRange.baseMipLevel;
        vkBarrier.subresourceRange.layerCount = barrier.subRange.layerCount;
        vkBarrier.subresourceRange.levelCount = barrier.subRange.levelCount;
        vkImageMemoryBarriers.push_back(vkBarrier);
        a_CommandBuffer->imageLayouts[**barrier.image] = barrier.newLayout;
    }
    a_CommandBuffer->pipelineBarrier(
        ConvertToVk(a_SrcStageMask),
        ConvertToVk(a_DstStageMask),
        ConvertToVk(a_DependencyFlags),
        vkMemoryBarriers,
        vkBufferMemoryBarriers,
        vkImageMemoryBarriers);
}

void SetVertexInput(
    const Command::Buffer::Handle& a_CommandBuffer,
    const std::vector<VertexAttributeDescription>& a_Attribs,
    const std::vector<VertexBindingDescription>& a_Bindings)
{
    std::vector<vk::VertexInputAttributeDescription2EXT> attribs(a_Attribs.size());
    std::vector<vk::VertexInputBindingDescription2EXT>   bindings(a_Bindings.size());
    std::transform(a_Attribs.begin(), a_Attribs.end(),
        attribs.begin(), [](auto& attrib) {
            return vk::VertexInputAttributeDescription2EXT(
                attrib.location,
                attrib.binding,
                ConvertToVk(attrib.format),
                attrib.offset);
        });
    std::transform(a_Bindings.begin(), a_Bindings.end(),
        bindings.begin(), [](auto& binding) {
            return vk::VertexInputBindingDescription2EXT(
                binding.binding,
                binding.stride,
                ConvertToVk(binding.inputRate),
                1); //TODO enable instanced rendering
        });
    a_CommandBuffer->setVertexInputEXT(bindings, attribs);
}

void TransitionImagesLayout(
    const Command::Buffer::Handle& a_CommandBuffer,
    const std::vector<ImageLayoutTransitionInfo>& a_Transitions)
{
    vk::PipelineStageFlags vkSrcStageFlags;
    vk::PipelineStageFlags vkDstStageFlags;    
    std::vector<vk::ImageMemoryBarrier> barriers(a_Transitions.size());
    for (auto i = 0u; i < barriers.size(); ++i) {
        auto& transition = a_Transitions.at(i);
        auto& image = *transition.image;
        auto& subResource = transition.subRange;
        auto& srcQueueFamily = transition.srcQueueFamilyIndex;
        auto& dstQueueFamily = transition.dstQueueFamilyIndex;
        auto& barrier = barriers.at(i);
        auto oldLayout = ConvertToVk(transition.oldLayout);
        auto newLayout = ConvertToVk(transition.newLayout);
        vkSrcStageFlags |= GetImageTransitionStage(oldLayout);
        vkDstStageFlags |= GetImageTransitionStage(newLayout);
        barrier.image = *image;
        barrier.subresourceRange = ConvertToVk(subResource);
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = srcQueueFamily;
        barrier.dstQueueFamilyIndex = dstQueueFamily;
        barrier.srcAccessMask = GetImageTransitionAccessMask(oldLayout);
        barrier.dstAccessMask = GetImageTransitionAccessMask(newLayout);
        a_CommandBuffer->imageLayouts[*image] = transition.newLayout;
    }
    a_CommandBuffer->pipelineBarrier(
        vkSrcStageFlags, vkDstStageFlags,
        vk::DependencyFlags{},
        {}, {}, barriers);
}

void TransitionImageLayout(
    const Command::Buffer::Handle& a_CommandBuffer,
    const ImageLayoutTransitionInfo& a_Transition)
{
    TransitionImagesLayout(
        a_CommandBuffer, { a_Transition });
}

void PushDescriptorSet(
    const Command::Buffer::Handle&         a_CommandBuffer,
    const Pipeline::Handle&                a_Pipeline,
    const std::vector<DescriptorSetWrite>& a_Writes)
{
    std::vector<vk::WriteDescriptorSet>   vkWriteInfos(a_Writes.size());
    std::vector<vk::DescriptorImageInfo>  vkImageInfos(a_Writes.size());
    std::vector<vk::DescriptorBufferInfo> vkBufferInfos(a_Writes.size());
    std::vector<ImageLayoutTransitionInfo> imageTransitions;

    for (auto i = 0u; i < vkWriteInfos.size(); ++i) {
        auto& writeInfo = a_Writes.at(i);
        auto& vkWriteInfo = vkWriteInfos.at(i);

        if (writeInfo.imageInfo.has_value()) {
            
            auto& info = writeInfo.imageInfo.value();
            auto& vkInfo = vkImageInfos.at(i);
            vkInfo = vk::DescriptorImageInfo(
                **info.sampler,
                **info.imageView,
                ConvertToVk(info.imageLayout));
            vkWriteInfo.pImageInfo = &vkInfo;
            auto& imageLayout = a_CommandBuffer->imageLayouts[**info.imageView->image];
            if (imageLayout != info.imageLayout) {
                ImageLayoutTransitionInfo imageTransition;
                imageTransition.image     = info.imageView->image;
                imageTransition.oldLayout = imageLayout;
                imageTransition.newLayout = info.imageLayout;
                imageTransition.subRange  = info.imageView->subResourceRange;
                TransitionImageLayout(a_CommandBuffer, { imageTransition });
            }
        }

        if (writeInfo.bufferInfo.has_value()) {
            auto& info = writeInfo.bufferInfo.value();
            auto& vkInfo = vkBufferInfos.at(i);
            vkInfo = vk::DescriptorBufferInfo(
                **info.buffer,
                info.offset,
                info.range);
            vkWriteInfo.pBufferInfo = &vkInfo;
        }
        vkWriteInfo.descriptorCount = writeInfo.dstCount;
        vkWriteInfo.descriptorType  = ConvertToVk(writeInfo.type);
        vkWriteInfo.dstArrayElement = writeInfo.dstArrayElement;
        vkWriteInfo.dstBinding      = writeInfo.dstBinding;
        if (writeInfo.dstSet != nullptr)
            vkWriteInfo.dstSet      = **writeInfo.dstSet;
    }
    a_CommandBuffer->pushDescriptorSetKHR(
        a_Pipeline->bindPoint,
        a_Pipeline->layout,
        0, vkWriteInfos);
}
}
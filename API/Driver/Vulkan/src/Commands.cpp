#include <VK/Buffer.hpp>
#include <VK/CommandBuffer.hpp>
#include <VK/Enums.hpp>
#include <VK/Flags.hpp>
#include <VK/FrameBuffer.hpp>
#include <VK/Image.hpp>
#include <VK/ImageView.hpp>
#include <VK/Pipeline.hpp>
#include <VK/Structs.hpp>

#include <OCRA/Core.hpp>

#include <vulkan/vulkan.h>

namespace OCRA::Command
{
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
void PipelineBarrier(
    const Buffer::Handle&     a_CommandBuffer,
    const PipelineStageFlags& a_SrcStageMask,
    const PipelineStageFlags& a_DstStageMask,
    const DependencyFlags&    a_DependencyFlags,
    const std::vector<MemoryBarrier>&       a_MemoryBarriers,
    const std::vector<BufferMemoryBarrier>& a_BufferMemoryBarriers,
    const std::vector<ImageMemoryBarrier>&  a_ImageMemoryBarriers)
{
    std::vector<vk::MemoryBarrier>       vkMemoryBarriers;
    std::vector<vk::BufferMemoryBarrier> vkBufferMemoryBarriers;
    std::vector<vk::ImageMemoryBarrier>  vkImageMemoryBarriers;
    vkMemoryBarriers.reserve(a_MemoryBarriers.size());
    for (const auto& barrier : a_MemoryBarriers) {
        vk::MemoryBarrier vkBarrier;
        vkBarrier.dstAccessMask = GetVkAccessMaskFlags(barrier.dstAccessMask);
        vkBarrier.srcAccessMask = GetVkAccessMaskFlags(barrier.srcAccessMask);
        vkMemoryBarriers.push_back(vkBarrier);
    }
    vkBufferMemoryBarriers.reserve(a_BufferMemoryBarriers.size());
    for (const auto& barrier : a_BufferMemoryBarriers) {
        vk::BufferMemoryBarrier vkBarrier;
        vkBarrier.dstAccessMask = GetVkAccessMaskFlags(barrier.dstAccessMask);
        vkBarrier.srcAccessMask = GetVkAccessMaskFlags(barrier.srcAccessMask);
        vkBarrier.dstQueueFamilyIndex = barrier.dstQueueFamilyIndex;
        vkBarrier.srcQueueFamilyIndex = barrier.srcQueueFamilyIndex;
        vkBarrier.buffer = **barrier.buffer;
        vkBarrier.offset = barrier.offset;
        vkBarrier.size   = barrier.size;
        vkBufferMemoryBarriers.push_back(vkBarrier);
    }
    vkImageMemoryBarriers.reserve(a_ImageMemoryBarriers.size());
    for (const auto& barrier : a_ImageMemoryBarriers) {
        vk::ImageMemoryBarrier vkBarrier;
        vkBarrier.image = **barrier.image;
        vkBarrier.newLayout     = vk::ImageLayout::eTransferDstOptimal;
        vkBarrier.dstAccessMask = GetVkAccessMaskFlags(barrier.dstAccessMask);
        vkBarrier.srcAccessMask = GetVkAccessMaskFlags(barrier.srcAccessMask);
        vkBarrier.dstQueueFamilyIndex = barrier.dstQueueFamilyIndex;
        vkBarrier.srcQueueFamilyIndex = barrier.srcQueueFamilyIndex;
        vkBarrier.subresourceRange.aspectMask     = GetVkImageAspectFlags(barrier.subRange.aspects);
        vkBarrier.subresourceRange.baseArrayLayer = barrier.subRange.baseArrayLayer;
        vkBarrier.subresourceRange.baseMipLevel   = barrier.subRange.baseMipLevel;
        vkBarrier.subresourceRange.layerCount     = barrier.subRange.layerCount;
        vkBarrier.subresourceRange.levelCount     = barrier.subRange.levelCount;
        vkImageMemoryBarriers.push_back(vkBarrier);
    }
    a_CommandBuffer->pipelineBarrier(
        GetVkPipelineStageFlags(a_SrcStageMask),
        GetVkPipelineStageFlags(a_DstStageMask),
        GetVkDependencyFlags(a_DependencyFlags),
        vkMemoryBarriers,
        vkBufferMemoryBarriers,
        vkImageMemoryBarriers);
}

void CopyImage(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Image::Handle& a_SrcImage,
    const Image::Handle& a_DstImage,
    const std::vector<ImageCopy>& a_Regions)
{
    std::vector<vk::ImageCopy> vkCopies(a_Regions.size());
    for (auto i = 0u; i < vkCopies.size(); ++i)
    {
        auto& copy = a_Regions.at(i);
        auto& vkCopy = vkCopies.at(i);
        vkCopy.extent = GetVkExtent3D(copy.extent);
        vkCopy.dstOffset      = GetVkOffset3D(copy.dstOffset);
        vkCopy.dstSubresource = GetVkImageSubresourceLayers(copy.dstSubresource);
        vkCopy.srcOffset      = GetVkOffset3D(copy.srcOffset);
        vkCopy.srcSubresource = GetVkImageSubresourceLayers(copy.srcSubresource);
    }
    a_CommandBuffer->copyImage(
        **a_SrcImage, vk::ImageLayout::eTransferSrcOptimal,
        **a_DstImage, vk::ImageLayout::eTransferDstOptimal,
        vkCopies);
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
        auto oldLayout = GetVkImageLayout(transition.oldLayout);
        auto newLayout = GetVkImageLayout(transition.newLayout);
        vkSrcStageFlags |= GetImageTransitionStage(oldLayout);
        vkDstStageFlags |= GetImageTransitionStage(newLayout);
        barrier.image = *image;
        barrier.subresourceRange = GetVkImageSubresourceRange(subResource);
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = srcQueueFamily;
        barrier.dstQueueFamilyIndex = dstQueueFamily;
        barrier.srcAccessMask = GetImageTransitionAccessMask(oldLayout);
        barrier.dstAccessMask = GetImageTransitionAccessMask(newLayout);
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

void ClearColorImage(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Image::Handle& a_Image,
    const ColorValue& a_Color,
    const std::vector<ImageSubresourceRange>& a_Ranges)
{
    static_assert(sizeof(ColorValue) == sizeof(vk::ClearColorValue));
    std::vector<vk::ImageSubresourceRange> vkRanges(a_Ranges.size());
    for (auto i = 0u; i < a_Ranges.size(); ++i)
        vkRanges.at(i) = GetVkImageSubresourceRange(a_Ranges.at(i));
    a_CommandBuffer->clearColorImage(**a_Image,
        vk::ImageLayout::eTransferDstOptimal,
        reinterpret_cast<const vk::ClearColorValue&>(a_Color),
        vkRanges);
}

void ExecuteCommandBuffer(
    const Buffer::Handle& a_CommandBuffer,
    const Buffer::Handle& a_SecondaryCommandBuffer)
{
    a_CommandBuffer->executeCommands({ **a_SecondaryCommandBuffer });
}

void BindPipeline(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Pipeline::Handle& a_Pipeline)
{
    auto& pipeline = *a_Pipeline;
    a_CommandBuffer->bindPipeline(vk::PipelineBindPoint(pipeline.bindPoint), *pipeline);
}

void BeginRenderPass(
    const Command::Buffer::Handle& a_CommandBuffer,
    const RenderPassBeginInfo& a_BeginInfo,
    const SubPassContents& a_SubPassContents)
{
    std::vector<VkClearValue> vkClearValues(a_BeginInfo.colorClearValues.size() + 1);
    for (auto i = 0u; i < a_BeginInfo.colorClearValues.size(); ++i)
    {
        auto& clearValue = a_BeginInfo.colorClearValues.at(i);
        auto& vkClearValue = vkClearValues.at(i);
        vkClearValue.color = GetVkClearColorValue(clearValue);
    }
    vkClearValues.back().depthStencil = GetVkClearDepthStencilValue(a_BeginInfo.depthStencilClearValue);
    VkRenderPassBeginInfo vkInfo{ VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO };
    vkInfo.clearValueCount = vkClearValues.size();
    vkInfo.framebuffer = *a_BeginInfo.framebuffer;
    vkInfo.pClearValues = vkClearValues.data();
}

static inline auto GetVkRenderingAttachmentInfo(const RenderingAttachmentInfo& a_Info) {
    vk::RenderingAttachmentInfo vkInfo;
    //vkInfo.clearValue = GetVkClearValue(a_Info.clearValue);
    //vkInfo.imageLayout = GetVkImageLayout(a_Info.imageLayout);
    //vkInfo.imageView = *a_Info.imageView;
    //vkInfo.loadOp = GetVkAttachmentLoadOp(a_Info.loadOp);
    ////vkInfo.resolveImageLayout
    ////vkInfo.resolveImageView
    ////vkInfo.resolveImageLayout
    //vkInfo.storeOp = GetVkAttachmentStoreOp(a_Info.storeOp);
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
    vkInfo.colorAttachmentCount = vkColorAttachments.size();
    //vkInfo.flags = GetVkRenderingFlags(a_Info.flags);
    //vkInfo.viewMask = a_Info.viewMask;
    vkInfo.flags = {};
    vkInfo.viewMask = 0;
    vkInfo.layerCount = a_Info.layerCount;
    vkInfo.renderArea = GetVkRect2D(a_Info.area);
    vkInfo.pColorAttachments = vkColorAttachments.data();
    vkInfo.pDepthAttachment = &vkDepthAttachment;
    vkInfo.pStencilAttachment = &vkStencilAttachment;
    a_CommandBuffer->beginRendering(vkInfo);
}

void EndRendering(const Command::Buffer::Handle& a_CommandBuffer)
{
    a_CommandBuffer->endRendering();
}
}
#include <VK/Buffer.hpp>
#include <VK/CommandBuffer.hpp>
#include <VK/Enums.hpp>
#include <VK/Flags.hpp>
#include <VK/Image.hpp>

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
    std::vector<VkBufferCopy> vkRegions;
    vkRegions.reserve(a_Regions.size());
    for (const auto& region : a_Regions) {
        VkBufferCopy vkRegion{};
        vkRegion.dstOffset = region.writeOffset;
        vkRegion.srcOffset = region.readOffset;
        vkRegion.size      = region.size;
        vkRegions.push_back(vkRegion);
    }
    vkCmdCopyBuffer(*a_CommandBuffer, *a_SrcBuffer, *a_DstBuffer, vkRegions.size(), vkRegions.data());
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
    std::vector<VkMemoryBarrier>       vkMemoryBarriers;
    std::vector<VkBufferMemoryBarrier> vkBufferMemoryBarriers;
    std::vector<VkImageMemoryBarrier>  vkImageMemoryBarriers;
    vkMemoryBarriers.reserve(a_MemoryBarriers.size());
    for (const auto& barrier : a_MemoryBarriers) {
        VkMemoryBarrier vkBarrier{ VK_STRUCTURE_TYPE_MEMORY_BARRIER };
        vkBarrier.dstAccessMask = GetVkAccessMaskFlags(barrier.dstAccessMask);
        vkBarrier.srcAccessMask = GetVkAccessMaskFlags(barrier.srcAccessMask);
        vkMemoryBarriers.push_back(vkBarrier);
    }
    vkBufferMemoryBarriers.reserve(a_BufferMemoryBarriers.size());
    for (const auto& barrier : a_BufferMemoryBarriers) {
        VkBufferMemoryBarrier vkBarrier{ VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER };
        vkBarrier.dstAccessMask = GetVkAccessMaskFlags(barrier.dstAccessMask);
        vkBarrier.srcAccessMask = GetVkAccessMaskFlags(barrier.srcAccessMask);
        vkBarrier.dstQueueFamilyIndex = barrier.dstQueueFamilyIndex;
        vkBarrier.srcQueueFamilyIndex = barrier.srcQueueFamilyIndex;
        vkBarrier.buffer = *barrier.buffer;
        vkBarrier.offset = barrier.offset;
        vkBarrier.size   = barrier.size;
        vkBufferMemoryBarriers.push_back(vkBarrier);
    }
    vkImageMemoryBarriers.reserve(a_ImageMemoryBarriers.size());
    for (const auto& barrier : a_ImageMemoryBarriers) {
        VkImageMemoryBarrier vkBarrier{ VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
        vkBarrier.image = *barrier.image;
        vkBarrier.newLayout     = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        vkBarrier.dstAccessMask = GetVkAccessMaskFlags(barrier.dstAccessMask);
        vkBarrier.srcAccessMask = GetVkAccessMaskFlags(barrier.srcAccessMask);
        vkBarrier.dstQueueFamilyIndex = barrier.dstQueueFamilyIndex;
        vkBarrier.srcQueueFamilyIndex = barrier.srcQueueFamilyIndex;
        vkBarrier.subresourceRange.aspectMask     = GetVkImageAspectFlags(barrier.subRange.aspect);
        vkBarrier.subresourceRange.baseArrayLayer = barrier.subRange.baseArrayLayer;
        vkBarrier.subresourceRange.baseMipLevel   = barrier.subRange.baseMipLevel;
        vkBarrier.subresourceRange.layerCount     = barrier.subRange.layerCount;
        vkBarrier.subresourceRange.levelCount     = barrier.subRange.levelCount;
        vkImageMemoryBarriers.push_back(vkBarrier);
    }
    vkCmdPipelineBarrier(*a_CommandBuffer,
        GetVkPipelineStageFlags(a_SrcStageMask),
        GetVkPipelineStageFlags(a_DstStageMask),
        GetVkDependencyFlags(a_DependencyFlags),
        vkMemoryBarriers.size(),       vkMemoryBarriers.data(),
        vkBufferMemoryBarriers.size(), vkBufferMemoryBarriers.data(),
        vkImageMemoryBarriers.size(),  vkImageMemoryBarriers.data());
}

static inline auto GetTransitionAccessMask(const VkImageLayout& a_Layout)
{
    switch (a_Layout)
    {
    case VK_IMAGE_LAYOUT_UNDEFINED:
    case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
        return VkAccessFlagBits(0);
    case VK_IMAGE_LAYOUT_GENERAL:
        return VkAccessFlagBits(
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT |
            VK_ACCESS_TRANSFER_WRITE_BIT | VK_ACCESS_TRANSFER_READ_BIT |
            VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_HOST_WRITE_BIT |
            VK_ACCESS_HOST_READ_BIT | VK_ACCESS_INPUT_ATTACHMENT_READ_BIT |
            VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
            VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT);
    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        return VkAccessFlagBits(
            VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
            VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT);

    case VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL:
    case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL:
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        return VkAccessFlagBits(
            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);
    case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL:
    case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL:
    case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL:
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
    case VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL:
        return VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;

    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        return VkAccessFlagBits(
            VK_ACCESS_SHADER_READ_BIT |
            VK_ACCESS_INPUT_ATTACHMENT_READ_BIT);

    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
        return VK_ACCESS_TRANSFER_READ_BIT;
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        return VK_ACCESS_TRANSFER_WRITE_BIT;
    case VK_IMAGE_LAYOUT_PREINITIALIZED:
        return VkAccessFlagBits(
            VK_ACCESS_HOST_WRITE_BIT |
            VK_ACCESS_TRANSFER_WRITE_BIT);

    case VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL:
        break;
    case VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL:
        break;
    case VK_IMAGE_LAYOUT_VIDEO_DECODE_DST_KHR:
        break;
    case VK_IMAGE_LAYOUT_VIDEO_DECODE_SRC_KHR:
        break;
    case VK_IMAGE_LAYOUT_VIDEO_DECODE_DPB_KHR:
        break;
    case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:
        return VK_ACCESS_MEMORY_READ_BIT;
    case VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT:
        break;
    case VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR:
        break;
    case VK_IMAGE_LAYOUT_ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT:
        break;
    }
    throw std::runtime_error("Error: unsupported layout transition!");
    return VK_ACCESS_FLAG_BITS_MAX_ENUM;
}

static inline auto GetTransitionStage(const VkImageLayout& a_Stage)
{
    switch (a_Stage)
    {
    case VK_IMAGE_LAYOUT_UNDEFINED:
        return VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    case VK_IMAGE_LAYOUT_GENERAL:
        return VK_PIPELINE_STAGE_ALL_COMMANDS_BIT;
    case VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        break;
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        break;
    case VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
        break;
    case VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        return VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    case VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL:
    case VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL:
        return VK_PIPELINE_STAGE_TRANSFER_BIT;
    case VK_IMAGE_LAYOUT_PREINITIALIZED:
        return VK_PIPELINE_STAGE_HOST_BIT;
    case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL:
        break;
    case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL:
        break;
    case VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL:
        break;
    case VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL:
        break;
    case VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL:
        break;
    case VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL:
        break;
    case VK_IMAGE_LAYOUT_READ_ONLY_OPTIMAL:
        break;
    case VK_IMAGE_LAYOUT_ATTACHMENT_OPTIMAL:
        return VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    case VK_IMAGE_LAYOUT_PRESENT_SRC_KHR:
        return VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    case VK_IMAGE_LAYOUT_VIDEO_DECODE_DST_KHR:
        break;
    case VK_IMAGE_LAYOUT_VIDEO_DECODE_SRC_KHR:
        break;
    case VK_IMAGE_LAYOUT_VIDEO_DECODE_DPB_KHR:
        break;
    case VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR:
        return VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
    case VK_IMAGE_LAYOUT_FRAGMENT_DENSITY_MAP_OPTIMAL_EXT:
        break;
    case VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR:
        break;
    case VK_IMAGE_LAYOUT_ATTACHMENT_FEEDBACK_LOOP_OPTIMAL_EXT:
        break;
    }
    throw std::runtime_error("Error: unsupported layout transition!");
    return VK_PIPELINE_STAGE_FLAG_BITS_MAX_ENUM;
}

void TransitionImagesLayout(
    const Command::Buffer::Handle& a_CommandBuffer,
    const std::vector<ImageLayoutTransitionInfo>& a_Transitions,
    const ImageLayout& a_OldLayout,
    const ImageLayout& a_NewLayout)
{
    auto oldLayout = GetVkImageLayout(a_OldLayout);
    auto newLayout = GetVkImageLayout(a_NewLayout);
    auto srcAccessMask = GetTransitionAccessMask(oldLayout);
    auto dstAccessMask = GetTransitionAccessMask(newLayout);
    std::vector<VkImageMemoryBarrier> barriers(a_Transitions.size(), { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER });
    for (auto i = 0u; i < barriers.size(); ++i) {
        auto& transition = a_Transitions.at(i);
        auto& image = *transition.image;
        auto& subResource = transition.subRange;
        auto& srcQueueFamily = transition.srcQueueFamilyIndex;
        auto& dstQueueFamily = transition.dstQueueFamilyIndex;
        auto& barrier = barriers.at(i);

        barrier.image = image;
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = srcQueueFamily;
        barrier.dstQueueFamilyIndex = dstQueueFamily;
        barrier.subresourceRange.aspectMask     = GetVkImageAspectFlags(subResource.aspect);
        barrier.subresourceRange.baseMipLevel   = subResource.baseMipLevel;
        barrier.subresourceRange.levelCount     = subResource.levelCount;
        barrier.subresourceRange.baseArrayLayer = subResource.baseArrayLayer;
        barrier.subresourceRange.layerCount     = subResource.layerCount;
        barrier.srcAccessMask = srcAccessMask;
        barrier.dstAccessMask = dstAccessMask;
    }
    vkCmdPipelineBarrier(*a_CommandBuffer,
        GetTransitionStage(oldLayout),
        GetTransitionStage(newLayout),
        0,
        0, nullptr,
        0, nullptr,
        barriers.size(), barriers.data());
}

void TransitionImageLayout(
    const Command::Buffer::Handle& a_CommandBuffer,
    const ImageLayoutTransitionInfo& a_Transition,
    const ImageLayout& a_OldLayout,
    const ImageLayout& a_NewLayout)
{
    TransitionImagesLayout(
        a_CommandBuffer, { a_Transition },
        a_OldLayout, a_NewLayout);
}

void ClearColorImage(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Image::Handle& a_Image,
    const ImageLayout& a_ImageLayout,
    const ColorValue& a_Color,
    const std::vector<ImageSubresourceRange>& a_Ranges)
{
    static_assert(sizeof(ColorValue) == sizeof(VkClearColorValue));
    VkClearColorValue color{};
    std::memcpy(&color, &a_Color, sizeof(ColorValue));
    std::vector<VkImageSubresourceRange> vkRanges(a_Ranges.size());
    for (auto i = 0u; i < a_Ranges.size(); ++i)
    {
        auto& range = a_Ranges.at(i);
        auto& vkRange = vkRanges.at(i);
        vkRange.baseArrayLayer = range.baseArrayLayer;
        vkRange.baseMipLevel   = range.baseMipLevel;
        vkRange.layerCount     = range.layerCount;
        vkRange.levelCount     = range.levelCount;
        vkRange.aspectMask     = GetVkImageAspectFlags(range.aspect);
    }
    vkCmdClearColorImage(*a_CommandBuffer, *a_Image, GetVkImageLayout(a_ImageLayout),
        &color, vkRanges.size(), vkRanges.data());
}
}
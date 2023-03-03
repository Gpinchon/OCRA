#include <VK/Buffer.hpp>
#include <VK/CommandBuffer.hpp>
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
        vkBarrier.dstAccessMask = GetVkAccessMaskFlags(barrier.dstAccessMask);
        vkBarrier.srcAccessMask = GetVkAccessMaskFlags(barrier.srcAccessMask);
        vkBarrier.dstQueueFamilyIndex = barrier.dstQueueFamilyIndex;
        vkBarrier.srcQueueFamilyIndex = barrier.srcQueueFamilyIndex;
        vkBarrier.image = *barrier.image;
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
        vkRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    }
    vkCmdClearColorImage(*a_CommandBuffer, *a_Image, VK_IMAGE_LAYOUT_GENERAL,
        &color, vkRanges.size(), vkRanges.data());
}
}
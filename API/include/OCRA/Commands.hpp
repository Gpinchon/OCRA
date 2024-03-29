#pragma once

#include <OCRA/Enums.hpp>
#include <OCRA/Handles.hpp>
#include <OCRA/Structs.hpp>

#include <vector>

namespace OCRA::Command {
/**
 * @brief Binds the specified pipeline to the command buffer
 */
void BindPipeline(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Pipeline::Handle& a_Pipeline);

/**
 * @brief Replaces the command buffer's bindings with the specified descriptor's.
 */
void BindDescriptorSet(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Pipeline::Handle& a_Pipeline,
    const Descriptor::Set::Handle& a_Descriptor,
    const uint32_t a_DynamicOffset);

void BindDescriptorSet(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Pipeline::Handle& a_Pipeline,
    const Descriptor::Set::Handle& a_Descriptor);

void CopyBufferToImage(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Buffer::Handle& a_SrcBuffer,
    const Image::Handle& a_DstImage,
    const std::vector<ImageBufferCopy>& a_Regions);

void CopyImageToBuffer(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Buffer::Handle& a_DstBuffer,
    const Image::Handle& a_SrcImage,
    const std::vector<ImageBufferCopy>& a_Regions);

/**
 * @brief Copies an image to an other,
 * a_SrcImage the source of the copy
 * a_SrcImageLayout the current Layout of the source image
 * a_DstImage the destination of the copy
 * a_DstImageLayout the current Layout of the destination image
 */
void CopyImage(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Image::Handle& a_SrcImage,
    const ImageLayout& a_SrcImageLayout,
    const Image::Handle& a_DstImage,
    const ImageLayout& a_DstImageLayout,
    const std::vector<ImageCopy>& a_Regions);

/**
 * @brief Copies an image to an other,
 * a_SrcImage should be in layout TransferSrcOptimal
 * a_DstImage should be in layout TransferDstOptimal
 */
void CopyImage(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Image::Handle& a_SrcImage,
    const Image::Handle& a_DstImage,
    const std::vector<ImageCopy>& a_Regions);

/**
 * @brief The same as CopyImage except it performs automatic conversion (eg. BGR to RGB)
 * a_SrcImage the source of the blit
 * a_SrcImageLayout the current layout of the source image
 * a_DstImage the destination of the blit
 * a_SrcImageLayout the current layout of the destination image
 */
void BlitImage(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Image::Handle& a_SrcImage,
    const ImageLayout& a_SrcImageLayout,
    const Image::Handle& a_DstImage,
    const ImageLayout& a_DstImageLayout,
    const std::vector<ImageBlit>& a_Blits,
    const Filter& a_Filter);

/**
 * @brief The same as CopyImage except it performs automatic conversion (eg. BGR to RGB)
 * a_SrcImage should be in layout TransferSrcOptimal
 * a_DstImage should be in layout TransferDstOptimal
 */
void BlitImage(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Image::Handle& a_SrcImage,
    const Image::Handle& a_DstImage,
    const std::vector<ImageBlit>& a_Blits,
    const Filter& a_Filter);

void GenerateMipMap(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Image::Handle& a_Image);

/**
 * On VK, this issues a Pipeline Barrier with appropriate flags
 * On OGL, might not do anything
 */
void TransitionImageLayout(
    const Command::Buffer::Handle& a_CommandBuffer,
    const ImageLayoutTransitionInfo& a_Transition);

/**
 * Same as TransitionImageLayout but for multiple images at once
 */
void TransitionImagesLayout(
    const Command::Buffer::Handle& a_CommandBuffer,
    const std::vector<ImageLayoutTransitionInfo>& a_Transitions);

/**
 * @brief Clears the specified image to the specified color
 * Image should be in layout Transfer Dst Optimal
 */
void ClearColorImage(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Image::Handle& a_Image,
    const ColorValue& a_Color,
    const std::vector<ImageSubresourceRange>& a_Ranges);

/**
 * @brief Pushes the specified descriptor to the command buffer's bindings
 * using the a_Pipeline's BindingPoint & Layout
 */
void PushDescriptorSet(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Pipeline::Handle& a_Pipeline,
    const std::vector<DescriptorSetWrite>& a_Writes);

/**
 * @brief Add a secondary Command Buffer to execute
 */
void ExecuteCommandBuffer(
    const Buffer::Handle& a_CommandBuffer,
    const Buffer::Handle& a_SecondaryCommandBuffer);

void PushConstants(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Pipeline::Handle& a_Pipeline,
    const uint8_t a_Offset,
    const std::vector<std::byte>& a_Data);

void SetScissor(
    const Command::Buffer::Handle& a_CommandBuffer,
    const uint32_t& a_FirstScissor,
    const std::vector<Rect2D>& a_Scissors);

void SetViewPort(
    const Command::Buffer::Handle& a_CommandBuffer,
    const uint32_t& a_FirstViewPort,
    const std::vector<ViewPort>& a_ViewPorts);

void BeginQuery(
    const Command::Buffer::Handle& a_CommandBuffer,
    const QueryPool::Handle& a_QueryPool,
    const uint32_t& a_QueryIndex);

// Draw commands
void Draw(
    const Command::Buffer::Handle& a_CommandBuffer,
    const uint32_t a_VertexCount,
    const uint32_t a_InstanceCount,
    const uint32_t a_FirstVertex,
    const uint32_t a_FirstInstance);

void DrawIndexed(
    const Command::Buffer::Handle& a_CommandBuffer,
    const uint32_t a_IndexCount,
    const uint32_t a_InstanceCount,
    const uint32_t a_FirstIndex,
    const uint32_t a_VertexOffset,
    const uint32_t a_FirstInstance);

/**
 * Update a buffer's contents from host memory
 * Size is limited to 65536
 */
void UpdateBuffer(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Buffer::Handle& a_DstBuffer,
    const size_t& a_Offset,
    const size_t& a_Size,
    const std::byte* a_Data);

/**
 * Update a buffer's contents from host memory
 * Size is limited to 65536
 */
void UpdateBuffer(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Buffer::Handle& a_DstBuffer,
    const size_t& a_Offset,
    const std::vector<std::byte>& a_Data);

/**
 * @brief Copies data from srcBuffer to dstBuffer
 */
void CopyBuffer(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Buffer::Handle& a_SrcBuffer,
    const OCRA::Buffer::Handle& a_DstBuffer,
    const std::vector<BufferCopyRegion>& a_Regions);

/**
 * @brief Bind specified Vertex Buffers to Render Pass currently bound to this Command Buffer
 */
void BindVertexBuffers(
    const Command::Buffer::Handle& a_CommandBuffer,
    const uint32_t a_FirstBinding,
    const std::vector<OCRA::Buffer::Handle>& a_VertexBuffers,
    const std::vector<uint64_t>& a_Offsets);

/**
 * @brief Bind specified Vertex Buffer to Render Pass currently bound to this Command Buffer
 */
void BindIndexBuffer(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Buffer::Handle& a_IndexBuffer,
    const uint64_t a_Offset,
    const IndexType a_IndexType);

void SetVertexInput(
    const Command::Buffer::Handle& a_CommandBuffer,
    const std::vector<VertexAttributeDescription>& a_Attribs,
    const std::vector<VertexBindingDescription>& a_Bindings);

void BeginRendering(
    const Command::Buffer::Handle& a_CommandBuffer,
    const RenderingInfo& a_RenderingInfo);

void EndRendering(
    const Command::Buffer::Handle& a_CommandBuffer);

void PipelineBarrier(
    const Buffer::Handle& a_CommandBuffer,
    const PipelineStageFlags& srcStageMask,
    const PipelineStageFlags& dstStageMask,
    const DependencyFlags& dependencyFlags,
    const std::vector<MemoryBarrier>& a_MemoryBarriers,
    const std::vector<BufferMemoryBarrier>& a_BufferMemoryBarriers,
    const std::vector<ImageMemoryBarrier>& a_ImageMemoryBarriers);
}

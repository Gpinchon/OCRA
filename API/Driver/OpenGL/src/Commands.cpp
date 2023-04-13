#include <GL/Flags.hpp>
#include <GL/CommandBuffer.hpp>
#include <GL/QueryPool.hpp>

#include <OCRA/Core.hpp>

namespace OCRA::Command
{
void BlitImage(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Image::Handle& a_SrcImage,
    const Image::Handle& a_DstImage,
    const std::vector<ImageBlit>& a_Blits,
    const Filter& a_Filter)
{}

void TransitionImageLayout(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Image::Handle& a_Image,
    const ImageSubresourceRange& a_SubResource,
    const ImageLayout& a_OldLayout,
    const ImageLayout& a_NewLayout)
{
    //TODO use glTextureBarrier, glMemoryBarrier and glMemoryBarrierByRegion when relevant
}

void TransitionImagesLayout(
    const Command::Buffer::Handle& a_CommandBuffer,
    const std::vector<Image::Handle>& a_Images,
    const std::vector<ImageSubresourceRange>& a_SubResources,
    const std::vector<ImageLayout>& a_OldLayouts,
    const std::vector<ImageLayout>& a_NewLayouts)
{}

void PipelineBarrier(
    const Buffer::Handle& a_CommandBuffer,
    const PipelineStageFlags& srcStageMask,
    const PipelineStageFlags& dstStageMask,
    const DependencyFlags& dependencyFlags,
    const std::vector<MemoryBarrier>&       a_MemoryBarriers,
    const std::vector<BufferMemoryBarrier>& a_BufferMemoryBarriers,
    const std::vector<ImageMemoryBarrier>&  a_ImageMemoryBarriers)
{
    a_CommandBuffer->PushCommand<GenericCommand>([
            memoryBarriers = a_MemoryBarriers,
            bufferMemoryBarriers = a_BufferMemoryBarriers,
            imageMemoryBarriers = a_ImageMemoryBarriers](const Buffer::ExecutionState&) {
        GLbitfield flags = 0;
        for (const auto& memoryBarrier : memoryBarriers) {
            flags |= GetGLBarrierAccessBits(memoryBarrier.dstAccessMask);
        }
        for (const auto& bufferMemoryBarrier : bufferMemoryBarriers) {
            flags |= GetGLBarrierAccessBits(bufferMemoryBarrier.dstAccessMask);
        }
        for (const auto& imageMemoryBarrier : imageMemoryBarriers) {
            flags |= GetGLBarrierAccessBits(imageMemoryBarrier.dstAccessMask);
        }
        glMemoryBarrierEXT(flags);
    });
}
void BeginQuery(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const QueryPool::Handle&        a_QueryPool,
    const uint32_t&                 a_QueryIndex)
{
    a_CommandBuffer->PushCommand<GenericCommand>([queryPool = a_QueryPool, queryIndex = a_QueryIndex](Buffer::ExecutionState& a_ExecutionState) {
        queryPool->Begin(queryIndex);
    });
}
void EndQuery(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const QueryPool::Handle&        a_QueryPool,
    const uint32_t&                 a_QueryIndex)
{
    a_CommandBuffer->PushCommand<GenericCommand>([queryPool = a_QueryPool, queryIndex = a_QueryIndex](Buffer::ExecutionState& a_ExecutionState) {
        queryPool->End(queryIndex);
    });
}
void SetScissor(
    const Command::Buffer::Handle& a_CommandBuffer,
    const uint32_t&                a_FirstScissor,
    const std::vector<Rect2D>&     a_Scissors)
{
    a_CommandBuffer->PushCommand<GenericCommand>([firstScissor = a_FirstScissor, scissors = a_Scissors](Buffer::ExecutionState& a_ExecutionState) {
        a_ExecutionState.dynamicStates.scissors.resize(scissors.size());
        for (auto index = firstScissor; index < scissors.size(); ++index)
            a_ExecutionState.dynamicStates.scissors.at(index) = scissors.at(index);
    });
}
void SetViewPort(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const uint32_t&                 a_FirstViewPort,
    const std::vector<ViewPort>&    a_ViewPorts)
{
    a_CommandBuffer->PushCommand<GenericCommand>([firstViewPort = a_FirstViewPort, viewports = a_ViewPorts](Buffer::ExecutionState& a_ExecutionState) {
        a_ExecutionState.dynamicStates.viewPorts.resize(viewports.size());
        for (auto index = firstViewPort; index < viewports.size(); ++index)
            a_ExecutionState.dynamicStates.viewPorts.at(index) = viewports.at(index);
    });
}
}
#include <OCRA/Core.hpp>

#include <GL/Flags.hpp>
#include <GL/Command/Buffer.hpp>

namespace OCRA::Command
{
void PipelineBarrier(
    const Buffer::Handle& a_CommandBuffer,
    const PipelineStageFlags& srcStageMask,
    const PipelineStageFlags& dstStageMask,
    const DependencyFlags& dependencyFlags,
    const std::vector<MemoryBarrier>&       a_MemoryBarriers,
    const std::vector<BufferMemoryBarrier>& a_BufferMemoryBarriers,
    const std::vector<ImageMemoryBarrier>&  a_ImageMemoryBarriers)
{
    a_CommandBuffer->PushCommand<GenericCommand>(
        [memoryBarriers = a_MemoryBarriers, bufferMemoryBarriers = a_BufferMemoryBarriers, imageMemoryBarriers = a_ImageMemoryBarriers](const Buffer::ExecutionState&) {
        for (const auto& memoryBarrier : memoryBarriers) {
            glMemoryBarrierEXT(GetGLBarrierAccessBits(memoryBarrier.dstAccessMask));
        }
        for (const auto& bufferMemoryBarrier : bufferMemoryBarriers) {
            glMemoryBarrierEXT(GetGLBarrierAccessBits(bufferMemoryBarrier.dstAccessMask));
        }
        for (const auto& imageMemoryBarrier : imageMemoryBarriers) {
            glMemoryBarrierEXT(GetGLBarrierAccessBits(imageMemoryBarrier.dstAccessMask));
        }
    });
}
}
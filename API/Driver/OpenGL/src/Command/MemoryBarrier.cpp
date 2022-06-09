#include <Command/MemoryBarrier.hpp>

#include <GL/Command/Buffer.hpp>

namespace OCRA::Command
{
static inline auto GetGLBarrierAccessBits(const AccessFlags& a_AccessFlags)
{
    GLbitfield barrier = 0;
    if ((a_AccessFlags & AccessFlagBits::ColorAttachmentRead) != 0)
        barrier |= GL_FRAMEBUFFER_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::ColorAttachmentWrite) != 0)
        barrier |= GL_FRAMEBUFFER_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::DepthStencilAttachmentRead) != 0)
        barrier |= GL_FRAMEBUFFER_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::DepthStencilAttachmentWrite) != 0)
        barrier |= GL_FRAMEBUFFER_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::HostRead) != 0)
        barrier |= GL_BUFFER_UPDATE_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::HostWrite) != 0)
        barrier |= GL_BUFFER_UPDATE_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::IndexRead) != 0)
        barrier |= GL_ELEMENT_ARRAY_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::IndirectCommandRead) != 0)
        barrier |= GL_COMMAND_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::InputAttachmentRead) != 0)
        barrier |= GL_SHADER_IMAGE_ACCESS_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::MemoryRead) != 0)
        barrier |= GL_BUFFER_UPDATE_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::MemoryWrite) != 0)
        barrier |= GL_BUFFER_UPDATE_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::ShaderRead) != 0)
        barrier |= GL_SHADER_STORAGE_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::ShaderWrite) != 0)
        barrier |= GL_SHADER_STORAGE_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::TransferRead) != 0)
        barrier |= GL_TEXTURE_UPDATE_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::TransferWrite) != 0)
        barrier |= GL_TEXTURE_UPDATE_BARRIER_BIT | GL_BUFFER_UPDATE_BARRIER_BIT;
    if ((a_AccessFlags & AccessFlagBits::UniformRead) != 0)
        barrier |= GL_UNIFORM_BARRIER_BIT;
    return barrier;
}
void PipelineBarrier(
    const Buffer::Handle& a_CommandBuffer,
    const Pipeline::StageFlags& srcStageMask,
    const Pipeline::StageFlags& dstStageMask,
    const DependencyFlags& dependencyFlags,
    const std::vector<MemoryBarrier>&       a_MemoryBarriers,
    const std::vector<BufferMemoryBarrier>& a_BufferMemoryBarriers,
    const std::vector<ImageMemoryBarrier>&  a_ImageMemoryBarriers)
{
    a_CommandBuffer->PushCommand(
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
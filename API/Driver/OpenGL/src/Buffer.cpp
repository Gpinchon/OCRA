#include <Buffer.hpp>
#include <Allocator.hpp>

#include <GL/Buffer.hpp>
#include <GL/Memory.hpp>
#include <GL/glew.h>

#include <cassert>

namespace OCRA::Buffer
{
Handle Create(
    const Device::Handle&       a_Device,
    const Info&                 a_Info,
    const AllocationCallback*   a_Allocator)
{
    return Handle(new Impl(a_Device, a_Info));
}
void BindMemory(const Device::Handle& a_Device, const Handle& a_Buffer, const Memory::Handle& a_Memory, const size_t& a_MemoryOffset)
{
    a_Buffer->memoryBinding.memory = a_Memory;
    a_Buffer->memoryBinding.offset = a_MemoryOffset;
}
}

#include <GL/Command/Buffer.hpp>
#include <GL/Command/ExecutionState.hpp>
#include <GL/Common/IndexType.hpp>

namespace OCRA::Command {
void CopyBuffer(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Buffer::Handle&    a_SrcBuffer,
    const OCRA::Buffer::Handle&    a_DstBuffer,
    const std::vector<BufferCopyRegion>&  a_Regions)
{
    a_CommandBuffer->PushCommand([
        srcBuffer = a_SrcBuffer,
        dstBuffer = a_DstBuffer,
        regions = a_Regions
    ](Buffer::ExecutionState& executionState) {
        auto& srcMemory = srcBuffer->memoryBinding;
        auto& dstMemory = dstBuffer->memoryBinding;
        for (const auto& region : regions) {
            glCopyNamedBufferSubData(
                srcMemory.memory->handle,
                dstMemory.memory->handle,
                region.readOffset + srcMemory.offset,
                region.writeOffset + dstMemory.offset,
                region.size);
        }
    });
}
void BindIndexBuffer(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Buffer::Handle& a_IndexBuffer,
    const uint64_t a_Offset,
    const IndexType a_IndexType)
{
    a_CommandBuffer->PushCommand([
        indexBuffer = a_IndexBuffer,
        offset = a_Offset, indexType = GetGLIndexType(a_IndexType)
    ](Buffer::ExecutionState& executionState) {
        executionState.renderPass.indexBufferBinding.buffer = indexBuffer;
        executionState.renderPass.indexBufferBinding.offset = offset;
        executionState.renderPass.indexBufferBinding.type   = indexType;
    });
    
}
void BindVertexBuffers(
    const Command::Buffer::Handle& a_CommandBuffer,
    const uint32_t a_FirstBinding,
    const std::vector<OCRA::Buffer::Handle>& a_VertexBuffers,
    const std::vector<uint64_t>& a_Offsets)
{
    a_CommandBuffer->PushCommand([
            firstBinding = a_FirstBinding,
            vertexBuffers = a_VertexBuffers,
            offsets = a_Offsets
        ](Buffer::ExecutionState& a_ExecutionState) {
            a_ExecutionState.renderPass.vertexInputBindings.resize(vertexBuffers.size());
            for (auto index = 0u; index < vertexBuffers.size(); ++index)
            {
                const auto binding = firstBinding + index;
                auto& vertexBuffer = vertexBuffers.at(index);
                a_ExecutionState.renderPass.vertexInputBindings.at(binding).buffer = vertexBuffer;
                a_ExecutionState.renderPass.vertexInputBindings.at(binding).offset = offsets.at(index);
            }
        });
}
}

#include <Buffer.hpp>
#include <Allocator.hpp>

#include <GL/Buffer.hpp>
#include <GL/Memory.hpp>
#include <GL/glew.h>

#include <cassert>

namespace OCRA::Buffer
{
struct Impl {
    Impl(Device::Handle a_Device, const Info& a_Info)
        : info(a_Info)
    {
        assert(info.usage != UsageFlagBits::None);
    }
    
    const Info      info;
    MemoryBinding   memoryBinding;
};
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
    a_Buffer->memoryBinding.memoryOffset = a_MemoryOffset;
}
const MemoryBinding& GetMemoryBinding(const Handle& a_Buffer)
{
    return a_Buffer->memoryBinding;
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
            Memory::GetGLHandle(srcMemory.memory),
            Memory::GetGLHandle(dstMemory.memory),
            region.readOffset + srcMemory.memoryOffset,
            region.writeOffset + dstMemory.memoryOffset,
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
        auto& memory = indexBuffer->memoryBinding;
        executionState.renderPass.indexBufferBinding.buffer = Memory::GetGLHandle(memory.memory);
        executionState.renderPass.indexBufferBinding.offset = offset + memory.memoryOffset;
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
                auto& memory = vertexBuffers.at(index)->memoryBinding;
                a_ExecutionState.renderPass.vertexInputBindings.at(binding).buffer = Memory::GetGLHandle(memory.memory);
                a_ExecutionState.renderPass.vertexInputBindings.at(binding).offset = offsets.at(index) + memory.memoryOffset;
            }
        });
}
}

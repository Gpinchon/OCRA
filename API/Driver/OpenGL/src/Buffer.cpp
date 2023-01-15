#include <OCRA/Buffer.hpp>
#include <OCRA/Allocator.hpp>

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
void Update(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const OCRA::Buffer::Handle&     a_DstBuffer,
    const size_t&                   a_Offset,
    const size_t&                   a_Size,
    const std::byte*                a_Data)
{
    assert(a_Size <= 65536);
    auto data = new std::byte[a_Size];
    std::memcpy(data, a_Data, a_Size);
    a_CommandBuffer->PushCommand([
        dstBuffer = a_DstBuffer,
        offset = a_Offset,
        size = a_Size,
        data
    ](Buffer::ExecutionState& executionState) {
        auto& dstMemory = dstBuffer->memoryBinding;
        glNamedBufferSubData(dstMemory.memory->handle, dstMemory.offset + offset, size, data);
        delete[] data;
    });
}
void Update(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const OCRA::Buffer::Handle&     a_DstBuffer,
    const size_t&                   a_Offset,
    const std::vector<std::byte>&   a_Data)
{
    assert(a_Data.size() <= 65536);
    a_CommandBuffer->PushCommand([
        dstBuffer = a_DstBuffer,
        offset = a_Offset,
        data = a_Data
    ](Buffer::ExecutionState& executionState) {
        auto& dstMemory = dstBuffer->memoryBinding;
        glNamedBufferSubData(dstMemory.memory->handle, dstMemory.offset + offset, data.size(), data.data());
    });
}
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
    const auto& indexBufferMemory = a_IndexBuffer->memoryBinding;
    a_CommandBuffer->PushCommand([
        indexBuffer = a_IndexBuffer,
        offset = a_Offset + indexBufferMemory.offset, indexType = GetGLIndexType(a_IndexType)
    ](Buffer::ExecutionState& a_ExecutionState) {
        a_ExecutionState.renderPass.indexBufferBinding.buffer = indexBuffer;
        a_ExecutionState.renderPass.indexBufferBinding.offset = offset;
        a_ExecutionState.renderPass.indexBufferBinding.type   = indexType;
    });
    
}

void BindVertexBuffers(
    const Command::Buffer::Handle& a_CommandBuffer,
    const uint32_t a_FirstBinding,
    const std::vector<OCRA::Buffer::Handle>& a_VertexBuffers,
    const std::vector<uint64_t>& a_Offsets)
{
    std::vector<uint64_t> offsets = a_Offsets;
    for (auto i = 0u; i < a_Offsets.size(); ++i) {
        const auto& vertexBuffer = a_VertexBuffers.at(i);
        const auto& vertexMemory = vertexBuffer->memoryBinding;
        offsets.at(i) += vertexMemory.offset; //offset binding according to memory offset
    }
    a_CommandBuffer->PushCommand([
            firstBinding = a_FirstBinding,
            vertexBuffers = a_VertexBuffers,
            offsets
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

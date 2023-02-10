////////////////////////////////////////////////////////////////////////////////
//  Buffer commands
////////////////////////////////////////////////////////////////////////////////

#include <GL/Command/Buffer.hpp>
#include <GL/Command/ExecutionState.hpp>
#include <GL/Command/CommandInterface.hpp>

#include <GL/Common/IndexType.hpp>

#ifdef _DEBUG
#include <cassert>
#endif

namespace OCRA::Command {
struct UpdateBufferCommand : CommandI {
    UpdateBufferCommand(
        const OCRA::Buffer::Handle& a_DstBuffer,
        const size_t& a_Offset,
        const size_t& a_Size,
        const std::byte* a_Data)
        : dstBuffer(a_DstBuffer)
        , offset(a_Offset)
        , size(a_Size)
    {
#ifdef _DEBUG
        assert(size <= 65536);
#endif
        std::copy(a_Data, a_Data + a_Size, data.data());
    }
    virtual void operator()(Buffer::ExecutionState&) override {
        auto& dstMemory = dstBuffer->memoryBinding;
        glNamedBufferSubData(dstMemory.memory->handle, dstMemory.offset + offset, size, data.data());
    }
    OCRA::Buffer::Handle dstBuffer;
    size_t offset;
    size_t size;
    std::array<std::byte, 65536> data;
};

struct CopyBufferCommand : CommandI {
    CopyBufferCommand(
        const OCRA::Buffer::Handle& a_SrcBuffer,
        const OCRA::Buffer::Handle& a_DstBuffer,
        const size_t a_RegionCount,
        const BufferCopyRegion* a_Regions)
        : srcBuffer(a_SrcBuffer)
        , dstBuffer(a_DstBuffer)
        , regionCount(a_RegionCount)
    {
#ifdef _DEBUG
        assert(regionCount <= 256);
#endif
        std::copy(a_Regions, a_Regions + a_RegionCount, regions.data());
    }
    virtual void operator()(Buffer::ExecutionState&) override {
        auto& srcMemory = srcBuffer->memoryBinding;
        auto& dstMemory = dstBuffer->memoryBinding;
        for (uint16_t i = 0; i < regionCount; ++i) {
            const auto& region = regions.at(i);
            glCopyNamedBufferSubData(
                srcMemory.memory->handle,
                dstMemory.memory->handle,
                region.readOffset + srcMemory.offset,
                region.writeOffset + dstMemory.offset,
                region.size);
        }
    }
    OCRA::Buffer::Handle srcBuffer;
    OCRA::Buffer::Handle dstBuffer;
    uint16_t regionCount;
    std::array<BufferCopyRegion, 256> regions;
};

struct BindIBOCommand : CommandI {
    BindIBOCommand(
        const OCRA::Buffer::Handle& a_IndexBuffer,
        const uint64_t a_Offset,
        const IndexType a_IndexType)
        : indexBuffer(a_IndexBuffer)
        , offset(a_Offset + indexBuffer->memoryBinding.offset)
        , indexType(GetGLIndexType(a_IndexType))
    {}
    virtual void operator()(Buffer::ExecutionState& a_ExecutionState) override {
        a_ExecutionState.renderPass.indexBufferBinding.buffer = indexBuffer;
        a_ExecutionState.renderPass.indexBufferBinding.offset = offset;
        a_ExecutionState.renderPass.indexBufferBinding.type = indexType;
    }
    const OCRA::Buffer::Handle indexBuffer;
    const uint32_t  offset;
    const GLenum    indexType;
};

struct BindVBOCommand : CommandI {
    BindVBOCommand(
        const uint32_t a_FirstBinding,
        const uint16_t a_Count,
        const uint64_t* a_Offsets,
        const OCRA::Buffer::Handle* a_Buffers)
        : firstBinding(a_FirstBinding)
        , count(a_Count)
    {
#ifdef _DEBUG
        assert(count <= 256);
#endif
        std::copy(a_Offsets, a_Offsets + a_Count, offsets.data());
        std::copy(a_Buffers, a_Buffers + a_Count, buffers.data());
    }
    virtual void operator()(Buffer::ExecutionState& a_ExecutionState) override {
        a_ExecutionState.renderPass.vertexInputBindings.resize(count);
        for (auto index = 0u; index < count; ++index)
        {
            const auto binding = firstBinding + index;
            a_ExecutionState.renderPass.vertexInputBindings.at(binding).buffer = buffers.at(index);
            a_ExecutionState.renderPass.vertexInputBindings.at(binding).offset = offsets.at(index);
        }
    }
    uint32_t                              firstBinding;
    uint16_t                              count;
    std::array<uint64_t, 256>             offsets;
    std::array<OCRA::Buffer::Handle, 256> buffers;
};
}

namespace OCRA::Command {
void UpdateBuffer(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const OCRA::Buffer::Handle&     a_DstBuffer,
    const size_t&                   a_Offset,
    const size_t&                   a_Size,
    const std::byte*                a_Data)
{
    a_CommandBuffer->PushCommand<UpdateBufferCommand>(a_DstBuffer, a_Offset, a_Size, a_Data);
}
void UpdateBuffer(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const OCRA::Buffer::Handle&     a_DstBuffer,
    const size_t&                   a_Offset,
    const std::vector<std::byte>&   a_Data)
{
    a_CommandBuffer->PushCommand<UpdateBufferCommand>(a_DstBuffer, a_Offset, a_Data.size(), a_Data.data());
}
void CopyBuffer(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Buffer::Handle&    a_SrcBuffer,
    const OCRA::Buffer::Handle&    a_DstBuffer,
    const std::vector<BufferCopyRegion>&  a_Regions)
{
    a_CommandBuffer->PushCommand<CopyBufferCommand>(a_SrcBuffer, a_DstBuffer, a_Regions.size(), a_Regions.data());
}

void BindIndexBuffer(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Buffer::Handle&    a_IndexBuffer,
    const uint64_t                 a_Offset,
    const IndexType                a_IndexType)
{
    a_CommandBuffer->PushCommand<BindIBOCommand>(a_IndexBuffer, a_Offset, a_IndexType);
}

void BindVertexBuffers(
    const Command::Buffer::Handle&           a_CommandBuffer,
    const uint32_t                           a_FirstBinding,
    const std::vector<OCRA::Buffer::Handle>& a_VertexBuffers,
    const std::vector<uint64_t>&             a_Offsets)
{
    const auto count = a_VertexBuffers.size();
    std::vector<uint64_t> offsets = a_Offsets;
    for (auto i = 0u; i < count; ++i) {
        const auto& vertexBuffer = a_VertexBuffers.at(i);
        const auto& vertexMemory = vertexBuffer->memoryBinding;
        offsets.at(i) += vertexMemory.offset; //offset binding according to memory offset
    }
    a_CommandBuffer->PushCommand<BindVBOCommand>(a_FirstBinding, count, offsets.data(), a_VertexBuffers.data());
}
}
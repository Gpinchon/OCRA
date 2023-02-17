////////////////////////////////////////////////////////////////////////////////
//  Buffer commands
////////////////////////////////////////////////////////////////////////////////

#include <GL/Command/Buffer.hpp>
#include <GL/Command/ExecutionState.hpp>
#include <GL/Command/CommandInterface.hpp>

#include <GL/Common/IndexType.hpp>

namespace OCRA::Command {
struct UpdateBufferCommand : CommandI {
    UpdateBufferCommand(
        std::pmr::memory_resource* a_MemoryResource,
        const OCRA::Buffer::Handle& a_DstBuffer,
        const size_t& a_Offset,
        const size_t& a_Size,
        const std::byte* a_Data)
        : dstBuffer(a_DstBuffer)
        , offset(a_Offset)
        , data(a_Data, a_Data + a_Size, a_MemoryResource)
    {}
    virtual void operator()(Buffer::ExecutionState&) override {
        auto& dstMemory = dstBuffer->memoryBinding;
        glNamedBufferSubData(dstMemory.memory->handle, dstMemory.offset + offset, data.size(), data.data());
    }
    OCRA::Buffer::Handle dstBuffer;
    size_t offset;
    std::pmr::vector<std::byte> data;
};

struct CopyBufferCommand : CommandI {
    CopyBufferCommand(
        std::pmr::memory_resource* a_MemoryResource,
        const OCRA::Buffer::Handle& a_SrcBuffer,
        const OCRA::Buffer::Handle& a_DstBuffer,
        const size_t a_RegionCount,
        const BufferCopyRegion* a_Regions)
        : srcBuffer(a_SrcBuffer)
        , dstBuffer(a_DstBuffer)
        , regions(a_Regions, a_Regions + a_RegionCount, a_MemoryResource)
    {}
    virtual void operator()(Buffer::ExecutionState&) override {
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
    }
    OCRA::Buffer::Handle srcBuffer;
    OCRA::Buffer::Handle dstBuffer;
    std::pmr::vector<BufferCopyRegion> regions;
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
        std::pmr::memory_resource* a_MemoryResource,
        const uint32_t a_FirstBinding,
        const uint16_t a_Count,
        const uint64_t* a_Offsets,
        const OCRA::Buffer::Handle* a_Buffers)
        : firstBinding(a_FirstBinding)
        , buffers(a_Count, a_MemoryResource)
    {
        for (auto index = 0u; index < buffers.size(); ++index) {
            const auto& buffer = a_Buffers[index];
            const auto  offset = a_Offsets[index] + buffer->memoryBinding.offset;
            buffers.at(index) = { offset, buffer };
        }
    }
    virtual void operator()(Buffer::ExecutionState& a_ExecutionState) override {
        a_ExecutionState.renderPass.vertexInputBindings.resize(buffers.size());
        for (auto index = 0u; index < buffers.size(); ++index) {
            const auto binding = firstBinding + index;
            a_ExecutionState.renderPass.vertexInputBindings.at(binding).offset = buffers.at(index).first;
            a_ExecutionState.renderPass.vertexInputBindings.at(binding).buffer = buffers.at(index).second;
        }
    }
    uint32_t firstBinding;
    std::pmr::vector<std::pair<uint64_t, OCRA::Buffer::Handle>> buffers;
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
    a_CommandBuffer->PushCommand<UpdateBufferCommand>(
        a_CommandBuffer->memoryResource,
        a_DstBuffer,
        a_Offset,
        a_Size,
        a_Data);
}
void UpdateBuffer(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const OCRA::Buffer::Handle&     a_DstBuffer,
    const size_t&                   a_Offset,
    const std::vector<std::byte>&   a_Data)
{
    a_CommandBuffer->PushCommand<UpdateBufferCommand>(
        a_CommandBuffer->memoryResource,
        a_DstBuffer,
        a_Offset,
        a_Data.size(),
        a_Data.data());
}
void CopyBuffer(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Buffer::Handle&    a_SrcBuffer,
    const OCRA::Buffer::Handle&    a_DstBuffer,
    const std::vector<BufferCopyRegion>&  a_Regions)
{
    a_CommandBuffer->PushCommand<CopyBufferCommand>(
        a_CommandBuffer->memoryResource,
        a_SrcBuffer,
        a_DstBuffer,
        a_Regions.size(),
        a_Regions.data());
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
    const std::vector<OCRA::Buffer::Handle>& a_Buffers,
    const std::vector<uint64_t>&             a_Offsets)
{
    a_CommandBuffer->PushCommand<BindVBOCommand>(
        a_CommandBuffer->memoryResource,
        a_FirstBinding,
        a_Buffers.size(),
        a_Offsets.data(),
        a_Buffers.data());
}
}
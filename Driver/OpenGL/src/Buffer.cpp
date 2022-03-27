#include <Buffer.hpp>
#include <Allocator.hpp>

#include <GL/glew.h>

#include <cassert>

namespace OCRA::Buffer
{
struct Impl {
    Impl(Device::Handle a_Device, const Info& a_Info)
        : info(a_Info)
    {
        assert(info.usage != UsageFlagsBits::None);
        allocationFlags |= (info.usage & UsageFlagsBits::TransferSrc) != 0 ? GL_MAP_READ_BIT : 0;
        allocationFlags |= (info.usage & UsageFlagsBits::TransferDst) != 0 ? GL_MAP_WRITE_BIT : 0;
        glCreateBuffers(1, &handle);
        glNamedBufferStorage(
            handle,
            info.size,
            nullptr,
            allocationFlags);
    }
    ~Impl()
    {
        glDeleteBuffers(1, &handle);
    }
    const Info  info;
    GLbitfield  allocationFlags{ 0 };
    GLuint      handle{ 0 };
};
Handle Create(
    const Device::Handle&       a_Device,
    const Info&                 a_Info,
    const AllocationCallback*   a_Allocator)
{
    return Handle(new Impl(a_Device, a_Info));
}
void* Map(
    Device::Handle  a_Device,
    Handle          a_Buffer,
    uint64_t        a_Offset,
    uint64_t        a_Length)
{
    return glMapNamedBufferRange(
        a_Buffer->handle,
        a_Offset,
        a_Length,
        a_Buffer->allocationFlags | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_UNSYNCHRONIZED_BIT);
}
void Unmap(
    Device::Handle  a_Device,
    Handle          a_Buffer)
{
    glUnmapNamedBuffer(a_Buffer->handle);
}
void Flush(
    Device::Handle  a_Device,
    Handle          a_Buffer,
    int32_t         a_Offset,
    uint64_t        a_Length)
{
    glFlushMappedNamedBufferRange(
        a_Buffer->handle,
        a_Offset,
        a_Length);
}
uint32_t GetGLHandle(const Handle& a_Buffer)
{
    return a_Buffer->handle;
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
     Command::Buffer::PushCommand(a_CommandBuffer, [
        srcBuffer = a_SrcBuffer,
        dstBuffer = a_DstBuffer,
        regions = a_Regions
    ](Buffer::ExecutionState& executionState) {
        for (const auto& region : regions) {
            glCopyNamedBufferSubData(
            srcBuffer->handle,
            dstBuffer->handle,
            region.readOffset,
            region.writeOffset,
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
    Command::Buffer::PushCommand(a_CommandBuffer, [
        indexBuffer = a_IndexBuffer,
        offset = a_Offset, indexType = GetGLIndexType(a_IndexType)
    ](Buffer::ExecutionState& executionState) {
        executionState.renderPass.indexBufferBinding.buffer = indexBuffer->handle;
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
        Command::Buffer::PushCommand(a_CommandBuffer, [
            firstBinding = a_FirstBinding,
            vertexBuffers = a_VertexBuffers,
            offsets = a_Offsets
        ](Buffer::ExecutionState& a_ExecutionState) {
            a_ExecutionState.renderPass.vertexInputBindings.resize(vertexBuffers.size());
            for (auto index = 0u; index < vertexBuffers.size(); ++index)
            {
                const auto binding = firstBinding + index;
                a_ExecutionState.renderPass.vertexInputBindings.at(binding).buffer = vertexBuffers.at(index)->handle;
                a_ExecutionState.renderPass.vertexInputBindings.at(binding).offset = offsets.at(index);
            }
        });
}
}
#include <IndexType.hpp>
#include <Command/Buffer.hpp>
#include <Buffer/Vertex.hpp>
#include <Buffer/Transfer.hpp>

#include <GL/Command/ExecutionState.hpp>
#include <GL/Command/Buffer.hpp>
#include <GL/Buffer/Vertex.hpp>
#include <GL/Buffer/Buffer.hpp>

#include <vector>

namespace OCRA::Command
{
void BindIndexBuffer(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Buffer::Vertex::Handle& a_IndexBuffer,
    const uint64_t a_Offset,
    const IndexType a_IndexType)
{
    Command::Buffer::PushCommand(a_CommandBuffer, [
        indexBuffer = a_IndexBuffer,
        offset = a_Offset, indexType = GetGLIndexType(a_IndexType)
    ](Buffer::ExecutionState& executionState) {
        const auto& bufferHandle{ Buffer::Vertex::GetBufferHandle(indexBuffer) };
        executionState.renderPass.indexBufferBinding.buffer = Buffer::GetGLHandle(bufferHandle);
        executionState.renderPass.indexBufferBinding.offset = offset;
        executionState.renderPass.indexBufferBinding.type   = indexType;
    });
    
}
void BindVertexBuffers(
    const Command::Buffer::Handle& a_CommandBuffer,
    const uint32_t a_FirstBinding,
    const uint32_t a_BindingCount,
    const std::vector<Buffer::Vertex::Handle>& a_VertexBuffers,
    const std::vector<uint64_t>& a_Offsets)
{
        Command::Buffer::PushCommand(a_CommandBuffer, [
            firstBinding = a_FirstBinding, bindingCount = a_BindingCount,
            vertexBuffers = a_VertexBuffers, offsets = a_Offsets
        ](Buffer::ExecutionState& executionState) {
            executionState.renderPass.vertexInputBindings.resize(bindingCount);
            for (auto index = 0u; index < bindingCount; ++index)
            {
                const auto& bufferHandle{ Buffer::Vertex::GetBufferHandle(vertexBuffers.at(index)) };
                const auto binding = firstBinding + index,
                executionState.renderPass.vertexInputBindings.at(binding).buffer = Buffer::GetGLHandle(bufferHandle);
                executionState.renderPass.vertexInputBindings.at(binding).offset = offsets.at(index);
            }
        });
}
}
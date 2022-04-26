#include <Command/Buffer.hpp>

#include <GL/Command/ExecutionState.hpp>
#include <GL/Command/Buffer.hpp>
#include <GL/Common/IndexType.hpp>
#include <GL/Buffer.hpp>
#include <GL/Common/BufferOffset.hpp>

namespace OCRA::Command
{
struct DrawArraysIndirectCommand {
    uint32_t count;
    uint32_t instanceCount;
    uint32_t first;
    uint32_t baseInstance;
};
struct DrawElementsIndirectCommand {
    uint32_t count;
    uint32_t instanceCount;
    uint32_t firstIndex;
    int32_t  baseVertex;
    uint32_t baseInstance;
};
//Draw commands
void Draw(
    const Command::Buffer::Handle& a_CommandBuffer,
    const uint32_t a_VertexCount,
    const uint32_t a_InstanceCount,
    const uint32_t a_FirstVertex,
    const uint32_t a_FirstInstance)
{
    DrawArraysIndirectCommand command{};
    command.count = a_VertexCount;
    command.instanceCount = a_InstanceCount;
    command.first = a_FirstVertex;
    command.baseInstance = a_FirstInstance;
    a_CommandBuffer->PushCommand([command = command](Buffer::ExecutionState& executionState) {
        glDrawArraysIndirect(
            executionState.renderPass.primitiveTopology,
            &command);
    });
}
void DrawIndexed(
    const Command::Buffer::Handle& a_CommandBuffer,
    const uint32_t a_IndexCount,
    const uint32_t a_InstanceCount,
    const uint32_t a_FirstIndex,
    const uint32_t a_VertexOffset,
    const uint32_t a_FirstInstance)
{
    DrawElementsIndirectCommand command{};
    command.count = a_IndexCount;
    command.instanceCount = a_InstanceCount;
    command.firstIndex = a_FirstIndex;
    command.baseVertex = a_VertexOffset;
    command.baseInstance = a_FirstInstance;
    a_CommandBuffer->PushCommand([command = command](Buffer::ExecutionState& executionState) {
            const auto& indexBufferBinding = executionState.renderPass.indexBufferBinding;
            const auto& indexMemoryBinding = indexBufferBinding.buffer->memoryBinding;
            const auto  indexMemoryOffset = BUFFER_OFFSET(indexBufferBinding.offset + indexMemoryBinding.offset);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexMemoryBinding.memory->handle);
            glDrawElementsInstancedBaseVertexBaseInstance(
                executionState.renderPass.primitiveTopology,
                command.count,
                indexBufferBinding.type,
                indexMemoryOffset,
                command.instanceCount,
                command.baseVertex,
                command.baseInstance);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    });
}
void DrawIndirect(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Buffer::Handle& a_Buffer,
    const uint64_t a_Offset,
    const uint32_t a_DrawCount,
    const uint32_t a_Stride)
{
    const auto offset = BUFFER_OFFSET(a_Buffer->memoryBinding.offset + a_Offset);
    a_CommandBuffer->PushCommand([
        buffer = a_Buffer, offset, drawCount = a_DrawCount, stride = a_Stride
        ](Buffer::ExecutionState& executionState) {
            glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buffer->memoryBinding.memory->handle);
            glMultiDrawArraysIndirect(
                executionState.renderPass.primitiveTopology,
                offset,
                drawCount,
                stride);
            glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
    });
}
void DrawIndexedIndirect(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Buffer::Handle& a_Buffer,
    const uint64_t a_Offset,
    const uint32_t a_DrawCount,
    const uint32_t a_Stride)
{
    const auto offset = BUFFER_OFFSET(a_Buffer->memoryBinding.offset + a_Offset);
    a_CommandBuffer->PushCommand([
        buffer = a_Buffer, offset, drawCount = a_DrawCount, stride = a_Stride
        ](Buffer::ExecutionState& executionState) {
            const auto& indexBufferBinding = executionState.renderPass.indexBufferBinding;
            const auto& indexMemoryBinding = indexBufferBinding.buffer->memoryBinding;
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexMemoryBinding.memory->handle);
            glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buffer->memoryBinding.memory->handle);
            glMultiDrawElementsIndirect(
                executionState.renderPass.primitiveTopology,
                indexBufferBinding.type,
                offset,
                drawCount,
                stride);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
    });
}
}
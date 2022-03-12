#include <Command/Buffer.hpp>
#include <Command/ExecutionState.hpp>

#include <GL/Command/Buffer.hpp>

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
    DrawArraysIndirectCommand command;
    command.count = a_VertexCount;
    command.instanceCount = a_InstanceCount;
    command.first = a_FirstVertex;
    command.baseInstance = a_FirstInstance;
    Command::Buffer::PushCommand(a_CommandBuffer, [
        command = command
        ](Buffer::ExecutionState& executionState) {
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
    DrawElementsIndirectCommand command;
    command.count = a_IndexCount;
    command.instanceCount = a_InstanceCount;
    //command.firstIndex = a_FirstIndex;
    command.baseVertex = a_VertexOffset;
    command.baseInstance = a_FirstInstance;
    Command::Buffer::PushCommand(a_CommandBuffer, [
        command = command,
        firstIndex = a_FirstIndex
        ](Buffer::ExecutionState& executionState) {
            const auto& indexBufferBinding = executionState.renderPass.indexBufferBinding;
            //OGL does not allow for offset when binding Element Array, emulate it through Draw Indirect
            const auto offset = indexBufferBinding.offset / GetIndexTypeSize(indexBufferBinding.indexType);
            command.firstIndex = firstIndex + offset;
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferBinding.buffer);
            glDrawElementsIndirect(
                executionState.renderPass.primitiveTopology,
                indexBufferBinding.indexType,
                &command);
    });
}
}
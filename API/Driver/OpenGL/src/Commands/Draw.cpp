////////////////////////////////////////////////////////////////////////////////
//  Drawing commands
////////////////////////////////////////////////////////////////////////////////

#include <OCRA/Command/Buffer.hpp>

#include <GL/Command/ExecutionState.hpp>
#include <GL/Command/Buffer.hpp>
#include <GL/Common/IndexType.hpp>
#include <GL/Common/BufferOffset.hpp>
#include <GL/Descriptor/Set.hpp>
#include <GL/Buffer.hpp>
#include <GL/Memory.hpp>
#include <GL/Pipeline/Pipeline.hpp>

namespace OCRA::Command
{
void ApplyPipelineStates(Buffer::ExecutionState& a_ExecutionState) {
    auto& pipelineState = a_ExecutionState.pipelineState.at(size_t(Pipeline::BindingPoint::Graphics));
    pipelineState.pipeline->Apply(a_ExecutionState);
    for (auto& binding : pipelineState.pushDescriptorSet)
        binding.Bind();
    if (pipelineState.descriptorSet != nullptr)
        pipelineState.descriptorSet->Bind();
}

struct DrawArraysIndirectCommand {
    uint32_t count;
    uint32_t instanceCount;
    uint32_t first;
    uint32_t baseInstance;
};

struct DrawBase : CommandI {
    DrawBase(OCRA::PushConstants& a_PushConstants)
        : pushConstants(a_PushConstants)
    {}
    virtual void operator()(Buffer::ExecutionState& a_ExecutionState) override {
        pushConstants.Bind();
        ApplyPipelineStates(a_ExecutionState);
    }
    OCRA::PushConstants& pushConstants;
};

struct DrawCommand : DrawBase {
    DrawCommand(
        OCRA::PushConstants& a_PushConstants,
        const uint32_t a_VertexCount,
        const uint32_t a_InstanceCount,
        const uint32_t a_FirstVertex,
        const uint32_t a_FirstInstance)
        : DrawBase(a_PushConstants)
        , vertexCount(a_VertexCount)
        , instanceCount(a_InstanceCount)
        , firstVertex(a_FirstVertex)
        , firstInstance(a_FirstInstance)
    {}
    virtual void operator()(Buffer::ExecutionState& a_ExecutionState) override {
        DrawBase::operator()(a_ExecutionState);
        glDrawArraysInstancedBaseInstance(
            a_ExecutionState.primitiveTopology,
            firstVertex, vertexCount, instanceCount, firstInstance);
    }
    const uint32_t vertexCount;
    const uint32_t instanceCount;
    const uint32_t firstVertex;
    const uint32_t firstInstance;
};

struct DrawIndexedCommand : DrawBase {
    DrawIndexedCommand(
        OCRA::PushConstants& a_PushConstants,
        const uint32_t a_IndexCount,
        const uint32_t a_InstanceCount,
        const uint32_t a_FirstIndex,
        const uint32_t a_VertexOffset,
        const uint32_t a_FirstInstance)
        : DrawBase(a_PushConstants)
        , indexCount(a_IndexCount)
        , instanceCount(a_InstanceCount)
        , firstIndex(a_FirstIndex)
        , baseVertex(a_VertexOffset)
        , baseInstance(a_FirstInstance)
    {}
    virtual void operator()(Buffer::ExecutionState& a_ExecutionState) {
        DrawBase::operator()(a_ExecutionState);
        const auto& indexBufferBinding = a_ExecutionState.renderPass.indexBufferBinding;
        const auto& indexMemoryBinding = indexBufferBinding.buffer->memoryBinding;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexMemoryBinding.memory->handle);
        glDrawElementsInstancedBaseVertexBaseInstance(
            a_ExecutionState.primitiveTopology,
            indexCount,
            indexBufferBinding.type,
            BUFFER_OFFSET(indexBufferBinding.offset),
            instanceCount,
            baseVertex,
            baseInstance);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    const uint32_t indexCount;
    const uint32_t instanceCount;
    const uint32_t firstIndex;
    const int32_t  baseVertex;
    const uint32_t baseInstance;
};

void Draw(
    const Command::Buffer::Handle& a_CommandBuffer,
    const uint32_t a_VertexCount,
    const uint32_t a_InstanceCount,
    const uint32_t a_FirstVertex,
    const uint32_t a_FirstInstance)
{
    a_CommandBuffer->PushCommand<DrawCommand>(
        a_CommandBuffer->pushConstants,
        a_VertexCount,
        a_InstanceCount,
        a_FirstVertex,
        a_FirstInstance);
}

void DrawIndexed(
    const Command::Buffer::Handle& a_CommandBuffer,
    const uint32_t a_IndexCount,
    const uint32_t a_InstanceCount,
    const uint32_t a_FirstIndex,
    const uint32_t a_VertexOffset,
    const uint32_t a_FirstInstance)
{
    a_CommandBuffer->PushCommand<DrawIndexedCommand>(
        a_CommandBuffer->pushConstants,
        a_IndexCount,
        a_InstanceCount,
        a_FirstIndex,
        a_VertexOffset,
        a_FirstInstance);
}

void DrawIndirect(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Buffer::Handle& a_Buffer,
    const uint64_t a_Offset,
    const uint32_t a_DrawCount,
    const uint32_t a_Stride)
{
    const auto offset = BUFFER_OFFSET(a_Buffer->memoryBinding.offset + a_Offset);
    a_CommandBuffer->PushCommand<GenericCommand>([
        &pushConstants = a_CommandBuffer->pushConstants,
        buffer = a_Buffer, offset, drawCount = a_DrawCount, stride = a_Stride
    ](Buffer::ExecutionState& a_ExecutionState) {
        pushConstants.Bind();
        ApplyPipelineStates(a_ExecutionState);
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buffer->memoryBinding.memory->handle);
        glMultiDrawArraysIndirect(
            a_ExecutionState.primitiveTopology,
            offset,
            drawCount,
            stride);
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
    });
}

void DrawIndexedIndirect(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Buffer::Handle& a_Buffer,
    const uint64_t a_Offset,
    const uint32_t a_DrawCount,
    const uint32_t a_Stride)
{
    const auto offset = BUFFER_OFFSET(a_Buffer->memoryBinding.offset + a_Offset);
    a_CommandBuffer->PushCommand<GenericCommand>([
        &pushConstants = a_CommandBuffer->pushConstants,
        buffer = a_Buffer, offset, drawCount = a_DrawCount, stride = a_Stride
    ](Buffer::ExecutionState& a_ExecutionState) {
        pushConstants.Bind();
        ApplyPipelineStates(a_ExecutionState);
        const auto& indexBufferBinding = a_ExecutionState.renderPass.indexBufferBinding;
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, buffer->memoryBinding.memory->handle);
        glMultiDrawElementsIndirect(
            a_ExecutionState.primitiveTopology,
            indexBufferBinding.type,
            offset,
            drawCount,
            stride);
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
    });
}
}
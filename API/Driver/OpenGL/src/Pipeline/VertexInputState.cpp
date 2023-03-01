#include <GL/Pipeline/VertexInputState.hpp>
#include <GL/ExecutionState.hpp>
#include <GL/Device.hpp>
#include <GL/Buffer.hpp>
#include <GL/Memory.hpp>

#include <GL/glew.h>

namespace OCRA::Pipeline {
CompileVertexInputState::CompileVertexInputState(
    const Device::Handle& a_Device,
    const PipelineVertexInputState& a_Info,
    const PipelineDynamicState& a_DynamicState)
    : device(a_Device)
    , primitiveRestartIndex(a_Info.primitiveRestartIndex)
    , bindingDescriptions(a_Info.bindingDescriptions)
{
    a_Device->PushCommand([this, &a_Info] {
        glGenVertexArrays(1, &handle);
        glBindVertexArray(handle);
        for (const auto& attribute : a_Info.attributeDescriptions) {
            glEnableVertexAttribArray(attribute.location);
            glVertexAttribFormat(
                attribute.location,
                attribute.format.size,
                GetGLVertexType(attribute.format.type),
                attribute.format.normalized,
                attribute.offset);
            glVertexAttribBinding(
                attribute.location,
                attribute.binding);
        }
        for (const auto& description : a_Info.bindingDescriptions) {
            //Is this binding divided by instance or by vertex ?
            const auto divideByInstance = description.inputRate == VertexInputRate::Instance;
            glVertexBindingDivisor(
                description.binding,
                divideByInstance ? 1 : 0);
        }
        glBindVertexArray(0);
    }, true);
}

CompileVertexInputState::CompileVertexInputState(const CompileVertexInputState& a_Other)
    : device(a_Other.device)
    , primitiveRestartIndex(a_Other.primitiveRestartIndex)
    , bindingDescriptions(a_Other.bindingDescriptions)
    , handle(std::exchange(a_Other.handle, 0))
{}

CompileVertexInputState::~CompileVertexInputState()
{
    if (handle == 0) return;
    device.lock()->PushCommand([handle = handle] {
        glDeleteVertexArrays(1, &handle);
    }, false);
}

void CompileVertexInputState::operator()(Command::Buffer::ExecutionState& a_ExecutionState) const
{
    glPrimitiveRestartIndex(primitiveRestartIndex);
    glBindVertexArray(handle);
    for (const auto& bindingDescription : bindingDescriptions) {
        const auto& vertexInput = a_ExecutionState.renderPass.vertexInputBindings.at(bindingDescription.binding);
        const auto& vertexBuffer = vertexInput.buffer;
        if (vertexBuffer == nullptr) {
            glBindVertexBuffer(
                bindingDescription.binding,
                0, 0,
                bindingDescription.stride);
        }
        else {
            const auto& vertexMemory = vertexBuffer->memoryBinding;
            glBindVertexBuffer(
                bindingDescription.binding,
                vertexMemory.memory->handle,
                vertexInput.offset,
                bindingDescription.stride);
        }
    }
}
}
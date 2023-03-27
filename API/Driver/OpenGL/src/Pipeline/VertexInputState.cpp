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
    , applyVertexInputs(!a_DynamicState.Contains(DynamicState::VertexInput))
    , primitiveRestartIndex(a_Info.primitiveRestartIndex)
    , bindingDescriptions(a_Info.bindingDescriptions)
{
    if (!applyVertexInputs) return;
    a_Device->PushCommand([this, attributeDescriptions = a_Info.attributeDescriptions] {
        glGenVertexArrays(1, &handle);
        glBindVertexArray(handle);
        for (const auto& attribute : attributeDescriptions) {
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
        for (const auto& description : bindingDescriptions) {
            //Is this binding divided by instance or by vertex ?
            const auto divideByInstance = description.inputRate == VertexInputRate::Instance;
            glVertexBindingDivisor(
                description.binding,
                divideByInstance ? 1 : 0);
        }
        glBindVertexArray(0);
    }, false);
}

CompileVertexInputState::CompileVertexInputState(const CompileVertexInputState& a_Other)
    : device(a_Other.device)
    , applyVertexInputs(a_Other.applyVertexInputs)
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
    if (!applyVertexInputs) return;
    a_ExecutionState.renderPass.vertexArray = handle;
    a_ExecutionState.renderPass.vertexInputBindings = bindingDescriptions;
}
}
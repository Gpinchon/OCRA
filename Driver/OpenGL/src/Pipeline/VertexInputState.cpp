#include <GL/Pipeline/VertexInputState.hpp>
#include <GL/Command/ExecutionState.hpp>
#include <GL/Device.hpp>
#include <GL/Buffer.hpp>
#include <GL/Memory.hpp>

#include <GL/glew.h>

namespace OCRA::Pipeline::VertexInputState {
struct VertexArray
{
    VertexArray(const Device::Handle& a_Device, const Info& a_Info)
        : device(a_Device)
    {
        a_Device->PushCommand([this, info = a_Info] {
            glGenVertexArrays(1, &handle);
            Bind();
            for (const auto& attribute : info.attributeDescriptions) {
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
            for (const auto& bindingDescription : info.bindingDescriptions) {
                //Is this binding divided by instance or by vertex ?
                const auto divideByInstance = bindingDescription.inputRate == BindingDescription::InputRate::Instance;
                glVertexBindingDivisor(
                    bindingDescription.binding,
                    divideByInstance ? 1 : 0);
            }
            Unbind();
        }, false);
    }
    ~VertexArray()
    {
        device.lock()->PushCommand([handle = handle] {
            glDeleteVertexArrays(1, &handle);
        }, false);
    }
    void Bind() const {
        glBindVertexArray(handle);
    }
    void Unbind() const {
        glBindVertexArray(0);
    }
    const Device::WeakHandle device;
    uint32_t handle{ 0 };
};

Compile::Compile(const Device::Handle& a_Device, const Info& a_Info, const DynamicState::Info& a_DynamicState)
    : device(a_Device)
    , info(a_Info)
{
    a_Device->PushCommand([this, info = a_Info] {
        glGenVertexArrays(1, &handle);
        glBindVertexArray(handle);
        for (const auto& attribute : info.attributeDescriptions) {
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
        for (const auto& bindingDescription : info.bindingDescriptions) {
            //Is this binding divided by instance or by vertex ?
            const auto divideByInstance = bindingDescription.inputRate == BindingDescription::InputRate::Instance;
            glVertexBindingDivisor(
                bindingDescription.binding,
                divideByInstance ? 1 : 0);
        }
        glBindVertexArray(0);
    }, true);
}

Compile::Compile(const Compile& a_Other)
    : device(a_Other.device)
    , info(a_Other.info)
    , handle(std::exchange(a_Other.handle, 0))
{}

Compile::~Compile()
{
    if (handle == 0) return;
    device.lock()->PushCommand([handle = handle] {
        glDeleteVertexArrays(1, &handle);
    }, false);
}

void Compile::operator()(Command::Buffer::ExecutionState& a_ExecutionState) const
{
    glPrimitiveRestartIndex(info.primitiveRestartIndex);
    glBindVertexArray(handle);
    for (const auto& bindingDescription : info.bindingDescriptions) {
        const auto& vertexInput = a_ExecutionState.renderPass.vertexInputBindings.at(bindingDescription.binding);
        const auto& vertexMemory = vertexInput.buffer->memoryBinding;
        //Is this binding divided by instance or by vertex ?
        glBindVertexBuffer(
            bindingDescription.binding,
            vertexMemory.memory->handle,
            vertexMemory.offset + vertexInput.offset,
            bindingDescription.stride);
    }
}
}
/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-03 22:10:56
*/
#pragma once

#include <Handle.hpp>
#include <Pipeline/VertexInputState.hpp>

#include <GL/Command/Buffer/ExecutionState.hpp>
#include <GL/Buffer/Vertex.hpp>
#include <GL/VertexType.hpp>
#include <GL/glew.h>

namespace OCRA::Pipeline::VertexInputState {
//compiles the specified Vertex Input State into a callback
//only the Vertex Attributes are compiled here, Vertex Bindings are compiled by Command::RenderPass::CompileGraphicStates
auto Compile(const Device::Handle& a_Device, const Info& a_Info, const DynamicState::Info& a_DynamicState)
{
    return [info = a_Info](Command::Buffer::ExecutionState& a_ExecutionState){
        glPrimitiveRestartIndex(info.primitiveRestartIndex);
        for (auto attribIndex = 0u; attribIndex < info.attributeDescriptionCount; ++attribIndex) {
            const auto& attribute = info.attributeDescriptions.at(attribIndex);
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
        for (auto bindingIndex = 0u; bindingIndex < info.bindingDescriptionCount; ++bindingIndex) {
            const auto& bindingDescription = info.bindingDescriptions.at(bindingIndex);
            const auto& vertexInputBinding = a_ExecutionState.renderPass.vertexInputBindings.at(bindingDescription.binding);
            glBindVertexBuffer(
                bindingDescription.binding,
                vertexInputBinding.buffer,
                vertexInputBinding.offset,
                bindingDescription.stride
            );
        }
    };
}
}
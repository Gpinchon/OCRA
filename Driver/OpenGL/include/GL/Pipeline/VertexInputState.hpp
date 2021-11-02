/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-03 22:10:56
*/
#pragma once

#include <Handle.hpp>
#include <Pipeline/VertexInputState.hpp>

#include <GL/Buffer/Vertex.hpp>
#include <GL/Pipeline/VAOPool.hpp>
#include <GL/glew.h>

namespace OCRA::Pipeline::VertexInputState {
//compiles the specified Vertex Input State into a callback
//only the Vertex Attributes are compiled here, Vertex Bindings are compiled by Command::RenderPass::CompileGraphicStates
struct Compile {
    Compile(const Device::Handle& a_Device, const Info& a_Info);
    void operator()(void) const noexcept
    {
        glPrimitiveRestartIndex(primitiveRestartIndex);
        glBindVertexArray(VAORef->handle);
    }
    const VAOPool::Reference VAORef;
    const Uint32 primitiveRestartIndex;
};
}
/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-03 22:10:56
*/
#pragma once

#include <Handle.hpp>
#include <Pipeline/VertexInputState.hpp>

#include <map>

#include <GL/Buffer/Vertex.hpp>
#include <GL/Vertex/VAOPool.hpp>
#include <GL/glew.h>

namespace OCRA::Pipeline::VertexInputState {
struct Impl {
    Impl(const Device::Handle& a_Device, VAOPool::Reference& a_VAOReference)
        : vaoRef(a_VAOReference)
    {
    }
    void operator()(const Device::Handle& a_Device)
    {
        auto vao { vaoRef.Get() };
        glBindVertexArray(vao->handle);
        if (vao->info.indexBufferDescription.buffer == 0)
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        else
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Buffer::Vertex::GetGLHandle(a_Device, vao->info.indexBufferDescription.buffer));
        glPrimitiveRestartIndex(vao->info.primitiveRestartIndex);
    }
    VAOPool::Reference vaoRef;
};
//compiles the specified Vertex Input State into a callback
Impl Compile(const Device::Handle& a_Device, const Info& a_Info);
}
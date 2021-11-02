/*
* @Author: gpinchon
* @Date:   2021-10-03 21:47:33
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-03 21:48:53
*/
#include <Handle.hpp>
#include <Pipeline/VertexInputState.hpp>

#include <GL/Pipeline/VertexInputState.hpp>
#include <GL/Pipeline/VAOPool.hpp>
#include <GL/glew.h>

#include <stdexcept>
#include <map>

namespace OCRA::Pipeline::VertexInputState {
static std::map<Device::Handle, VAOPool> s_VAOs;
Compile::Compile(const Device::Handle& a_Device, const Info& a_Info)
    : primitiveRestartIndex(a_Info.primitiveRestartIndex)
    , VAORef([a_Device, a_Info]{
        auto& vaoPool{ s_VAOs[a_Device] };
        if (auto vao{ vaoPool.FindSimilar(a_Info) }; vao.Get() != nullptr) //try to find a similar VAO
            return vao;
        else if (auto vao{ vaoPool.FindFree() }; vao.Get() != nullptr) //we couldn't find similar VAO, try to find free VAO
        {
            vao->Set(a_Device, a_Info);
            return vao;
        }
        else throw std::runtime_error("No more free VAO");
        }())
{
}
}
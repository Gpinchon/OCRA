/*
* @Author: gpinchon
* @Date:   2021-10-03 21:47:33
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-03 21:48:53
*/
#include <GL/Pipeline/VertexInputState.hpp>
#include <GL/Vertex/VAOPool.hpp>

#include <stdexcept>

namespace OCRA::Pipeline::VertexInputState {
Impl Compile(const Device::Handle& a_Device, const Info& a_Info)
{
    static std::map<Device::Handle, VAOPool> s_VAOs;
    auto& vaoPool { s_VAOs[a_Device] };
    //try to find a similar VAO
    if (auto vao { vaoPool.FindSimilar(a_Info) }; vao.Get() != nullptr)
        return Impl(a_Device, vao);
    //we couldn't find similar VAO, try to find free VAO
    if (auto vao { vaoPool.FindFree() }; vao.Get() != nullptr) {
        vao->Set(a_Device, a_Info);
        return Impl(a_Device, vao);
    }
    throw std::runtime_error("No more free VAO");
}
}
/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-03 22:10:56
*/
#pragma once

#include <Handle.hpp>
#include <Pipeline/VertexInputState.hpp>

#include <GL/Common/VertexType.hpp>
#include <GL/WeakHandle.hpp>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Pipeline::DynamicState {
struct Info;
}

namespace OCRA::Command::Buffer {
struct ExecutionState;
}

namespace OCRA::Pipeline::VertexInputState {
//compiles the specified Vertex Input State into a callback
//only the Vertex Attributes are compiled here, Vertex Bindings are compiled by on execution
struct Compile
{
    Compile(const Device::Handle& a_Device, const Info& a_Info, const DynamicState::Info& a_DynamicState);
    Compile(const Compile& a_Other);
    ~Compile();
    void operator()(Command::Buffer::ExecutionState&) const;
    const Info info;
    const Device::WeakHandle device;
    mutable uint32_t handle{ 0 };
};
}
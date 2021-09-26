/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 15:03:29
*/
#pragma once

#include <Handle.hpp>
#include <Pipeline/VertexInputState.hpp>

#include <functional>

namespace OCRA::Pipeline::VertexInputState {
//compiles the specified Vertex Input State into a callback
std::function<void()> Compile(const Device::Handle& a_Device, const Info& a_Info);
}
/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 15:03:29
*/
#pragma once

#include <Handle.hpp>
#include <Pipeline/VertexInputState.hpp>

#include <GL/glew.h>
#include <functional>

namespace OCRA::Pipeline::VertexInputState {
struct Impl {
};
//compiles the specified Vertex Input State into a callback
inline auto Compile(const Device::Handle& a_Device, const Info& a_Info) {

	glVertexAttribBinding(
		GLuint attribindex,
		GLuint bindingindex);
}
}
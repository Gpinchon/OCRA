/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/
#pragma once

#include <Handle.hpp>
#include <Pipeline/Graphics.hpp>
#include <RenderPass.hpp>

#include <GL/glew.h>

#include <cassert>
#include <functional>
#include <mutex>

namespace OCRA::Command
{
struct VertexInputBinding {
	GLuint buffer{ 0 };
	Uint64 offset{ 0 };
};
struct IndexBufferBinding : VertexInputBinding {
	GLenum indexType{ GL_NONE };
};
struct RenderPass
{
	RenderPassBeginInfo beginInfo;
	static constexpr auto MaxVertexBuffers = 32;
	std::array<VertexInputBinding, MaxVertexBuffers> vertexInputBindings;
	IndexBufferBinding indexBufferBinding;
};
}

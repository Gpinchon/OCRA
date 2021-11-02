/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/
#pragma once

#include <Handle.hpp>
#include <Pipeline/Graphics.hpp>
#include <Command/RenderPass.hpp>
#include <Pipeline/BindingPoint.hpp>

#include <GL/Pipeline/Graphics.hpp>

#include <cassert>
#include <functional>

#include <GL/glew.h>

namespace OCRA::Command
{
struct VertexBufferBinding {
	static constexpr auto MaxVertexBuffers = 32;
	Uint32 firstBinding{ 0 };
	Uint32 bindingCount{ 0 };
	std::array<GLuint, MaxVertexBuffers> vertexBuffers{ 0 };
	std::array<Uint64, MaxVertexBuffers> offsets{ 0 };
};
struct IndexBufferBinding {
	GLuint buffer{ 0 };
	Uint64 offset{ 0 };
	GLenum indexType{ GL_NONE };
};
struct RenderPass
{
	std::function<void()> CompileGraphicStates(const Device::Handle& a_Device) {
		const auto graphicsPipeline{ pipelines.at(size_t(Pipeline::BindingPoint::Graphics)) };
		if(graphicsPipeline == 0) return;
		return Pipeline::Graphics::Compile(a_Device, graphicsPipeline);
	}
	RenderPassBeginInfo beginInfo;
	std::array<Pipeline::Handle, size_t(Pipeline::BindingPoint::MaxValue)> pipelines{ 0 };
	VertexBufferBinding vertexBinding;
	IndexBufferBinding indexBuffer;
};
}
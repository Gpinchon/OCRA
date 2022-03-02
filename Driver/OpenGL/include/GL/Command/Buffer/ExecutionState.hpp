#pragma once

#include <Handle.hpp>
#include <Scalar.hpp>
#include <Pipeline/BindingPoint.hpp>
#include <Command/RenderPass.hpp>
#include <Common/DepthBounds.hpp>
#include <Common/Rect2D.hpp>
#include <Common/Vec4.hpp>
#include <Compare.hpp>
#include <Stencil.hpp>

#include <GL/glew.h>

#include <array>

namespace OCRA::Command::Buffer
{
struct DynamicStates {
	static constexpr auto MaxViewPorts = 64;
	std::array<Rect<2, GLfloat>, MaxViewPorts> viewport;
	std::array<Rect<2, GLint>, MaxViewPorts> scissor;
	Vec<4, GLfloat> blendConstants{ 0, 0, 0, 0 };
	GLfloat lineWidth{ 0.f };
	GLfloat depthBiasConstantFactor{ 0.f };
	GLfloat depthBiasClamp{ 0.f };
	GLfloat depthBiasSlopeFactor{ 0.f };
	DepthBounds<double> depthBounds{ 0, 0 };
	bool depthTestEnable { true };
    bool depthWriteEnable { true };
	bool depthBoundsTestEnable{ true };
    Compare::Operation depthCompareOp { Compare::Operation::Less };
    bool stencilTestEnable { false };
    bool rasterizerDiscardEnable { false };
    Stencil::OpState frontStencilOP;
    Stencil::OpState backStencilOP;
};
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
	constexpr static auto MaxVertexBuffers = 32;
	std::array<VertexInputBinding, MaxVertexBuffers> vertexInputBindings;
	IndexBufferBinding indexBufferBinding;
};
struct ExecutionState {
	bool				once{ false };
	RenderPass			renderPass{};
	Uint32				subpassIndex{ 0 };
	std::array<Pipeline::Handle, size_t(Pipeline::BindingPoint::MaxValue)> pipelineState{};
	DynamicStates		dynamicStates;
};
}
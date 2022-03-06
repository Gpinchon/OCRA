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
#include <ViewPort.hpp>

#include <GL/glew.h>

#include <array>

namespace OCRA::Command::Buffer
{
constexpr auto MaxViewPorts = 64;
constexpr auto MaxScissors = 64;
struct DynamicStates {
	Uint8 viewPortsCount { 0 };
	std::array<ViewPort, MaxViewPorts> viewPorts;
	Uint8 scissorsCount { 0 };
	std::array<Rect<2, GLint>, MaxScissors> scissors;
	Vec<4, GLfloat> blendConstants{ 0, 0, 0, 0 };
	GLfloat lineWidth{ 0.f };
	GLfloat depthBiasConstantFactor{ 0.f };
	GLfloat depthBiasSlopeFactor{ 0.f };
	GLfloat depthBiasClamp{ 0.f };
	DepthBounds<double> depthBounds{ 0, 0 };
	bool depthTestEnable { true };
    bool depthWriteEnable { true };
	bool depthBoundsTestEnable { true };
    GLenum depthCompareOp { GL_LESS };
    bool stencilTestEnable { false };
    bool rasterizerDiscardEnable { false };
    Stencil::GLOpState frontStencilOP;
    Stencil::GLOpState backStencilOP;
    GLenum primitiveTopology{ GL_NONE };
    bool primitiveRestartEnable { false };
    GLenum cullMode{ GL_BACK };
    GLenum frontFace{ GL_CCW };
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
	constexpr static auto MaxVertexBuffers = 32;
	RenderPassBeginInfo beginInfo;
	std::array<VertexInputBinding, MaxVertexBuffers> vertexInputBindings;
	IndexBufferBinding indexBufferBinding;
	GLenum primitiveTopology{ GL_NONE };
};
struct ExecutionState {
	bool				once{ false };
	RenderPass			renderPass{};
	Uint32				subpassIndex{ 0 };
	std::array<Pipeline::Handle, size_t(Pipeline::BindingPoint::MaxValue)> pipelineState{};
	DynamicStates		dynamicStates;
};
}
#pragma once

#include <Handle.hpp>
#include <Common/ClearValue.hpp>
#include <Common/Compare.hpp>
#include <Common/DepthBounds.hpp>
#include <Common/Stencil.hpp>
#include <Common/Rect2D.hpp>
#include <Common/Vec4.hpp>

#include <Common/ViewPort.hpp>
#include <FrameBuffer.hpp>
#include <RenderPass.hpp>

#include <GL/Common/Stencil.hpp>
#include <GL/glew.h>

#include <vector>
#include <array>

OCRA_DECLARE_HANDLE(OCRA::Pipeline);

namespace OCRA::Command::Buffer
{
struct DynamicStates {
	std::vector<ViewPort>		viewPorts;
	std::vector<iRect2D>		scissors;
	Vec<4, GLfloat> blendConstants{ 0, 0, 0, 0 };
	GLfloat lineWidth{ 0.f };
	GLfloat depthBiasConstantFactor{ 0.f };
	GLfloat depthBiasSlopeFactor{ 0.f };
	GLfloat depthBiasClamp{ 0.f };
	DepthBounds<double> depthBounds{ 0, 0 };
	bool depthBiasEnable{ false };
	bool depthTestEnable { true };
    bool depthWriteEnable { true };
	bool depthBoundsTestEnable { false };
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
	uint64_t offset{ 0 };
};
struct IndexBufferBinding : VertexInputBinding {
	GLenum type{ GL_NONE };
};
struct RenderPass
{
	OCRA::RenderPass::Handle	renderPass;
	FrameBuffer::Handle			framebuffer;
	Rect2D						renderArea;
	std::vector<ClearValue>			clearValues;
	std::vector<VertexInputBinding>	vertexInputBindings;
	IndexBufferBinding				indexBufferBinding;
	GLenum							primitiveTopology{ GL_NONE };
};
struct ExecutionState {
	bool				once{ false };
	RenderPass			renderPass{};
	uint32_t			subpassIndex{ 0 };
	std::array<OCRA::Pipeline::Handle, size_t(OCRA::Pipeline::BindingPoint::MaxValue)> pipelineState{};
	DynamicStates		dynamicStates;
};
}
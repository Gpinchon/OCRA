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
#include <algorithm>

OCRA_DECLARE_HANDLE(OCRA::Pipeline);
OCRA_DECLARE_HANDLE(OCRA::Buffer);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::Set);

namespace OCRA::Command::Buffer
{
struct DynamicStates {
	std::vector<ViewPort>		viewPorts;
	std::vector<Rect2D> 		scissors;
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
	OCRA::Buffer::Handle buffer{ 0 };
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
	std::vector<ColorValue> 	colorClearValues;
    float                   	depthClearValue;
    int32_t                	    stencilClearValue;
	std::vector<VertexInputBinding>	vertexInputBindings;
	IndexBufferBinding				indexBufferBinding;
};
struct DescriptorSets {
	Pipeline::Layout::Handle pipelineLayout;
	std::vector<Descriptor::Set::Handle> descriptorSets;
	std::vector<uint32_t> dynamicOffset;
	bool operator!=(const DescriptorSets& a_Other) {
		return
			pipelineLayout != a_Other.pipelineLayout ||
			descriptorSets.size() != a_Other.descriptorSets.size() ||
			!std::equal(descriptorSets.begin(), descriptorSets.end(), a_Other.descriptorSets.begin()) ||
			dynamicOffset.size() != a_Other.dynamicOffset.size() ||
			!std::equal(dynamicOffset.begin(), dynamicOffset.end(), a_Other.dynamicOffset.begin());
	}
};
struct ExecutionState {
	ExecutionState() {
		descriptorSets.fill({});
		pipelineState.fill(nullptr);
		lastPipelineState.fill(nullptr);
	}
	bool				once{ false };
	RenderPass			renderPass{};
	uint32_t			subpassIndex{ uint32_t(-1) };
	std::array<DescriptorSets, size_t(OCRA::Pipeline::BindingPoint::MaxValue)> descriptorSets{};
	std::array<OCRA::Pipeline::Handle, size_t(OCRA::Pipeline::BindingPoint::MaxValue)> pipelineState{};
	std::array<OCRA::Pipeline::Handle, size_t(OCRA::Pipeline::BindingPoint::MaxValue)> lastPipelineState{};
	DynamicStates		dynamicStates{};
	GLenum				primitiveTopology{ GL_NONE };
};
}
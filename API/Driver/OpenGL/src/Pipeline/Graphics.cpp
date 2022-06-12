/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 15:03:53
*/
#include <Pipeline/Pipeline.hpp>
#include <Pipeline/Graphics.hpp>

#include <GL/Pipeline/Pipeline.hpp>
#include <GL/Pipeline/ColorBlendState.hpp>
#include <GL/Pipeline/DepthStencilState.hpp>
#include <GL/Pipeline/InputAssemblyState.hpp>
#include <GL/Pipeline/MultisampleState.hpp>
#include <GL/Pipeline/RasterizationState.hpp>
#include <GL/Pipeline/ShaderPipelineState.hpp>
#include <GL/Pipeline/TessellationState.hpp>
#include <GL/Pipeline/ViewPortState.hpp>
#include <GL/Pipeline/VertexInputState.hpp>
#include <GL/RenderPass.hpp>

#include <functional>
#include <cassert>

namespace OCRA::Pipeline::Graphics {
struct Impl : Pipeline::Impl {
	Impl(const Device::Handle& a_Device, const Info& a_Info)
	: Pipeline::Impl(BindingPoint::Graphics, a_Info.layout)
	, info(a_Info)
	{
		Apply = [
			this,
			colorBlendState = ColorBlendState::Compile(a_Device, info.colorBlendState, info.dynamicState),
			depthStencilState = DepthStencilState::Compile(a_Device, info.depthStencilState, info.dynamicState),
			inputAssemblyState = InputAssemblyState::Compile(a_Device, info.inputAssemblyState, info.dynamicState),
			multisampleState = MultisampleState::Compile(a_Device, info.multisampleState, info.dynamicState),
			rasterizationState = RasterizationState::Compile(a_Device, info.rasterizationState, info.dynamicState),
			shaderPipelineState = ShaderPipelineState::Compile(a_Device, info.shaderPipelineState, info.dynamicState),
			tessellationState = TessellationState::Compile(a_Device, info.tessellationState, info.dynamicState),
			viewportState = ViewPortState::Compile(a_Device, info.viewPortState, info.dynamicState),
			vertexInputState = VertexInputState::Compile(a_Device, info.vertexInputState, info.dynamicState)
		](Command::Buffer::ExecutionState& a_ExecutionState) {
			assert(info.renderPass == a_ExecutionState.renderPass.renderPass);
			colorBlendState(a_ExecutionState);
			depthStencilState(a_ExecutionState);
			inputAssemblyState(a_ExecutionState);
			multisampleState(a_ExecutionState);
			rasterizationState(a_ExecutionState);
			shaderPipelineState(a_ExecutionState);
			tessellationState(a_ExecutionState);
			viewportState(a_ExecutionState);
			vertexInputState(a_ExecutionState);
			a_ExecutionState.subpassIndex = info.subPass;
			a_ExecutionState.renderPass.renderPass->BeginSubPass(a_ExecutionState);
		};
	}
    const Info info;
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info) {
	return Handle(new Graphics::Impl(a_Device, a_Info));
}
const Info& GetInfo(const Device::Handle& a_Device, const Handle& a_Handle) {
	assert(a_Handle->bindingPoint == BindingPoint::Graphics);
    return std::static_pointer_cast<Impl>(a_Handle)->info;
}
}
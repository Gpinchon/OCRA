/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 15:03:53
*/
#include <Handle.hpp>
#include <Pipeline/Graphics.hpp>

#include <GL/Pipeline/Pipeline.hpp>
#include <GL/Pipeline/ColorBlendState.hpp>
#include <GL/Pipeline/DepthStencilState.hpp>
#include <GL/Pipeline/InputAssemblyState.hpp>
#include <GL/Pipeline/MultisampleState.hpp>
#include <GL/Pipeline/ShaderPipelineState.hpp>
#include <GL/Pipeline/TessellationState.hpp>
#include <GL/Pipeline/ViewPortState.hpp>
#include <GL/Pipeline/VertexInputState.hpp>

#include <functional>

namespace OCRA::Pipeline::Graphics {
struct Impl : Base {
	Impl(const Device::Handle& a_Device, const Info& a_Info)
	: info(a_Info)
	{}
	auto Compile(const Device::Handle& a_Device)
	{
		if (compiled) return callback;
		callback = [
				colorBlendState = ColorBlendState::Compile(a_Device, info.colorBlendState, info.dynamicState),
				depthStencilState = DepthStencilState::Compile(a_Device, info.depthStencilState, info.dynamicState),
				inputAssemblyState = InputAssemblyState::Compile(a_Device, info.inputAssemblyState, info.dynamicState),
				multisampleState = MultisampleState::Compile(a_Device, info.multisampleState, info.dynamicState),
				rasterizationState = RasterizationState::Compile(a_Device, info.shaderPipelineState, info.dynamicState),
				shaderPipelineState = ShaderPipelineState::Compile(a_Device, info.shaderPipelineState, info.dynamicState),
				tessellationState = TessellationState::Compile(a_Device, info.tessellationState, info.dynamicState),
				viewportState = ViewPortState::Compile(a_Device, info.viewPortState, info.dynamicState),
				vertexInputState = VertexInputState::Compile(a_Device, info.vertexInputState, info.dynamicState)
			](Command::Buffer::ExecutionState& a_ExecutionState){
				colorBlendState(a_ExecutionState);
				depthStencilState(a_ExecutionState);
				inputAssemblyState(a_ExecutionState);
				multisampleState(a_ExecutionState);
				rasterizationState(a_ExecutionState);
				shaderPipelineState(a_ExecutionState);
				tessellationState(a_ExecutionState);
				viewportState(a_ExecutionState);
				vertexInputState(a_ExecutionState);
			};
		compiled = true;
		return callback;
	}
	virtual void Execute(const Device::Handle& a_Device, const Handle& a_Handle, Command::Buffer::ExecutionState& a_ExecutionState) override
	{
		Compile(a_Device)(a_ExecutionState);
	}
    const Info info;
	bool compiled{ false };
    std::function<void(Command::Buffer::ExecutionState&)> callback;
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info) {
	return Pipeline::Create(a_Device, [a_Device, a_Info]() { return new Impl(a_Device, a_Info); });
}
void Destroy(const Device::Handle& a_Device, const Handle& a_Handle) {
	Pipeline::Destroy(a_Device, a_Handle);
}
const Info& GetInfo(const Device::Handle& a_Device, const Handle& a_Handle) {
    return static_cast<Impl&>(Get(a_Handle)).info;
}
}
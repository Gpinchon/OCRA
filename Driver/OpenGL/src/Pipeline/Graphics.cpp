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
#include <GL/Pipeline/MultisampleState.hpp>
#include <GL/Pipeline/ShaderPipelineState.hpp>
#include <GL/Pipeline/TessellationState.hpp>
#include <GL/Pipeline/ViewPortState.hpp>
#include <GL/Pipeline/VertexInputState.hpp>

#include <functional>
#include <vector>
#include <memory>

namespace OCRA::Pipeline::Graphics {
struct Impl : Base {
	Impl(const Device::Handle& a_Device, const Info& a_Info)
	: info(a_Info)
	{}
	virtual std::function<void()> Compile(const Device::Handle& a_Device) override
	{
		if (!compiled)
		{
			callback = [
				colorBlendState = ColorBlendState::Compile(a_Device, info.colorBlendState),
				depthStencilState = DepthStencilState::Compile(a_Device, info.depthStencilState),
				multisampleState = MultisampleState::Compile(a_Device, info.multiSampleState),
				shaderPipelineState = ShaderPipelineState::Compile(a_Device, info.shaderPipelineState),
				tessellationState = TessellationState::Compile(a_Device, info.tessellationState),
				viewportState = ViewPortState::Compile(a_Device, info.viewPortState),
				vertexInputState = VertexInputState::Compile(a_Device, info.vertexInputState)
			](){
				colorBlendState();
				depthStencilState();
				multisampleState();
				shaderPipelineState();
				tessellationState();
				viewportState();
				vertexInputState();
			};
			compiled = true;
		}
		return callback;
	}
    const Info info;
	bool compiled{ false };
    std::function<void()> callback;
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
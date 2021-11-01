/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 15:03:53
*/
#include <Handle.hpp>
#include <Pipeline/Graphics.hpp>

#include <functional>
#include <map>
#include <vector>

#include <GL/Pipeline/ColorBlendState.hpp>
#include <GL/Pipeline/DepthStencilState.hpp>
#include <GL/Pipeline/MultisampleState.hpp>
#include <GL/Pipeline/ShaderPipelineState.hpp>
#include <GL/Pipeline/TessellationState.hpp>
#include <GL/Pipeline/ViewPortState.hpp>
#include <GL/Pipeline/VertexInputState.hpp>
#include <GL/Pipeline/Graphics.hpp>

namespace OCRA::Pipeline::Graphics {
struct Impl {
	Impl(const Device::Handle& a_Device, const Info& a_Info)
	: info(a_Info)
	{}
	void operator()(void) const
	{
		colorBlendState();
		depthStencilState();
		multisampleState();
		shaderPipelineState();
		tessellationState();
		viewportState();
		vertexInputState();
	}
	std::function<void()> Compile(const Device::Handle& a_Device) {
		if (!compiled)
		{
			colorBlendState = ColorBlendState::Compile(a_Device, a_Info.colorBlendState);
			depthStencilState = DepthStencilState::Compile(a_Device, a_Info.depthStencilState);
			multisampleState = MultisampleState::Compile(a_Device, a_Info.multiSampleState);
			shaderPipelineState = ShaderPipelineState::Compile(a_Device, a_Info.shaderPipelineState)
			tessellationState = TessellationState::Compile(a_Device, a_Info.tessellationState);
			viewportState = ViewPortState::Compile(a_Device, a_Info.viewPortState);
			vertexInputState = VertexInputState::Compile(a_Device, a_Info.vertexInputState);
			compiled = true;
		}
		return *this;
	}
    const Info info;
    bool compiled = false;
    std::function<void()> colorBlendState;
    std::function<void()> depthStencilState;
	std::function<void()> multisampleState;
	std::function<void()> shaderPipelineState;
	std::function<void()> tessellationState;
	std::function<void()> viewportState;
	std::function<void()> vertexInputState;
};
static Handle s_CurrentHandle = 0;
static std::map<Handle, Impl> s_GraphicsPipelines;
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
    ++s_CurrentHandle;
    s_GraphicsPipelines.emplace(s_CurrentHandle, Impl(a_Device, a_Info));
    return s_CurrentHandle;
}
void Destroy(const Device::Handle& a_Device, const Handle& a_Handle)
{
    s_GraphicsPipelines.erase(a_Handle);
}
const Info& GetInfo(const Device::Handle& a_Device, const Handle& a_Handle)
{
    return s_GraphicsPipelines.at(a_Handle).info;
}
std::function<void()> Compile(const Device::Handle& a_Device, const Handle& a_GraphicsPipeline)
{
	return s_GraphicsPipelines.at(a_GraphicsPipeline).Compile(a_Device);
}
}
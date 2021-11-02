/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-11-01 14:14:17
*/
#include <Handle.hpp>
#include <Pipeline/Graphics.hpp>

#include <functional>
#include <map>
#include <vector>

#include <GL/Pipeline/ColorBlendState.hpp>
#include <GL/Pipeline/DepthStencilState.hpp>
#include <GL/Pipeline/Graphics.hpp>
#include <GL/Pipeline/MultisampleState.hpp>
#include <GL/Pipeline/ShaderPipelineState.hpp>
#include <GL/Pipeline/TessellationState.hpp>
#include <GL/Pipeline/VertexInputState.hpp>
#include <GL/Pipeline/ViewPortState.hpp>

namespace OCRA::Pipeline::Graphics {
struct Impl {
    Impl(const Device::Handle& a_Device, const Info& a_Info)
        : info(a_Info)
    {
    }
    std::function<void()> Compile(const Device::Handle& a_Device)
    {
        auto colorBlendState = ColorBlendState::Compile(a_Device, info.colorBlendState);
        auto depthStencilState = DepthStencilState::Compile(a_Device, info.depthStencilState);
        auto multisampleState = MultisampleState::Compile(a_Device, info.multiSampleState);
        auto shaderPipelineState = ShaderPipelineState::Compile(a_Device, info.shaderPipelineState);
        auto tessellationState = TessellationState::Compile(a_Device, info.tessellationState);
        auto viewportState = ViewPortState::Compile(a_Device, info.viewPortState);
        auto vertexInputState = VertexInputState::Compile(a_Device, info.vertexInputState);
        return [colorBlendState = colorBlendState,
                depthStencilState = depthStencilState,
                multisampleState = multisampleState,
                shaderPipelineState = shaderPipelineState,
                tessellationState = tessellationState,
                viewportState = viewportState,
                vertexInputState = vertexInputState]() {
            colorBlendState();
            depthStencilState();
            multisampleState();
            shaderPipelineState();
            tessellationState();
            viewportState();
            vertexInputState();
        };
    }
    void ApplyGraphicsStates(const Device::Handle& a_Device)
    {
        if (!compiled) applyGraphicsStatesCB = Compile(a_Device);
        applyGraphicsStatesCB();
    }
    const Info info;
    bool compiled{ false };
    std::function<void()> applyGraphicsStatesCB;
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
void ApplyGraphicsStates(const Device::Handle& a_Device, const Handle& a_GraphicsPipeline)
{
    s_GraphicsPipelines.at(a_GraphicsPipeline).ApplyGraphicsStates(a_Device);
}
}
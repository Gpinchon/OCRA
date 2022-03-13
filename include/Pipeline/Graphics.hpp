/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:28:40
*/
#pragma once

#include <Device.hpp>
#include <RenderPass.hpp>
#include <Pipeline/ColorBlendState.hpp>
#include <Pipeline/DepthStencilState.hpp>
#include <Pipeline/DynamicState.hpp>
#include <Pipeline/InputAssemblyState.hpp>
#include <Pipeline/MultisampleState.hpp>
#include <Pipeline/RasterizationState.hpp>
#include <Pipeline/ShaderPipelineState.hpp>
#include <Pipeline/TessellationState.hpp>
#include <Pipeline/VertexInputState.hpp>
#include <Pipeline/ViewPortState.hpp>

#include <vector>

namespace OCRA::Pipeline::Graphics {
struct Info { //describes a graphics pipeline with each stage
	RenderPass::Handle          renderPass{ 0 }; //a handle to a previously created render pass
    ShaderPipelineState::Info   shaderPipelineStates;
    ColorBlendState::Info       colorBlendState;
    DepthStencilState::Info     depthStencilState;
    InputAssemblyState::Info    inputAssemblyState;
    MultisampleState::Info      multisampleState;
    RasterizationState::Info    rasterizationState;
    ShaderPipelineState::Info   shaderPipelineState;
    TessellationState::Info     tessellationState;
    VertexInputState::Info      vertexInputState;
    ViewPortState::Info         viewPortState;
    DynamicState::Info          dynamicState;
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info);
const Info& GetInfo(const Device::Handle& a_Device, const Handle& a_Handle);
}
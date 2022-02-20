/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:28:40
*/
#pragma once

#include <Handle.hpp>
#include <Pipeline/ColorBlendState.hpp>
#include <Pipeline/DepthStencilState.hpp>
#include <Pipeline/InputAssemblyState.hpp>
#include <Pipeline/MultisampleState.hpp>
#include <Pipeline/RasterizationState.hpp>
#include <Pipeline/ShaderPipelineState.hpp>
#include <Pipeline/TessellationState.hpp>
#include <Pipeline/VertexInputState.hpp>
#include <Pipeline/ViewPortState.hpp>
#include <Scalar.hpp>

#include <vector>

//TODO decide if this is relevant
namespace OCRA::Pipeline::DynamicState {
enum class DynamicState {
};
struct Info {
    std::vector<DynamicState> dynamicStates;
};
}

namespace OCRA::Pipeline::Graphics {
struct Info { //describes a graphics pipeline with each stage
	RenderPass::Handle renderPass{ 0 }; //a handle to a previously created render pass
	Int8 subPass{ -1 }; //-1 means no subpass
    ShaderPipelineState::Info shaderPipelineState;
    ColorBlendState::Info colorBlendState;
    DepthStencilState::Info depthStencilState;
    InputAssemblyState::Info inputAssemblyState;
    MultisampleState::Info multiSampleState;
    RasterizationState::Info rasterizationState;
    TessellationState::Info tessellationState;
    VertexInputState::Info vertexInputState;
    ViewPortState::Info viewPortState;
    DynamicState::Info dynamicState;
};
}
/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:28:40
*/
#pragma once

#include <Pipeline/ColorBlendState.hpp>
#include <Pipeline/DepthStencilState.hpp>
#include <Pipeline/InputAssemblyState.hpp>
#include <Pipeline/MultisampleState.hpp>
#include <Pipeline/RasterizationState.hpp>
#include <Pipeline/ShaderStage.hpp>
#include <Pipeline/TessellationState.hpp>
#include <Pipeline/VertexInputState.hpp>
#include <Pipeline/ViewPortState.hpp>

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
    std::vector<ShaderStage::Info> shaderStage;
    ColorBlendState::Info colorBlendState;
    DepthStencilState::Info depthStencilState;
    InputAssemblyState::Info inputAssemblyState;
    MultisampleState::Info multiSampleState;
    RasterizationState::Info rasterizationState;
    TessellationState::Info tesselationState;
    VertexInputState::Info vertexInputState;
    ViewPortState::Info viewPortState;
    DynamicState::Info dynamicState;
};
}
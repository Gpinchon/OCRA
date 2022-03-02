#pragma once

#include <list>
#include <algorithm>

//TODO decide if this is relevant
namespace OCRA::Pipeline::DynamicState {
enum class State {
	Viewport,
	Scissor,
	LineWidth,
	DepthBias,
	BlendConstants,
	DepthBounds,
	StencilCompareMask,
	StencilWriteMask,
	StencilReference,
	//VK 1.3
	CullMode,
	FrontFace,
	PrimitiveTopology,
	ViewportWithCount,
	ScissorWithCount,
	VertexInputBindingStride,
	DepthTestEnable,
	DepthWriteEnable,
	DepthBoundsTestEnable,
	DepthCompareOP,
	StencilTestEnable,
	StencilOP,
	RasterizerDiscardEnable,
	DepthBiasEnable,
	PrimitiveRestartEnable,
	MaxValue
};
struct Info {
    std::list<State> dynamicStates;
    bool Contains(const State a_DynamicState) {
        return std::find(dynamicStates.begin(), dynamicStates.end(), a_DynamicState) != dynamicStates.end();
    }
};
}
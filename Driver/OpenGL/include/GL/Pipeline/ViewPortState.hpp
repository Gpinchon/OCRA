/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:56
*/
#pragma once

#include <Pipeline/ViewPortState.hpp>
#include <Common/Rect2D.hpp>

#include <array>

#include <GL/Command/Buffer/ExecutionState.hpp>
#include <GL/glew.h>

namespace OCRA::Pipeline::ViewPortState {
inline auto Compile(const Device::Handle& a_Device, const Info& a_Info, const DynamicState::Info& a_DynamicState)
{
	return [
		info = a_Info,
		dynamicViewport(a_DynamicState.Contains(DynamicState::State::Viewport)),
		dynamicScissor(a_DynamicState.Contains(DynamicState::State::Scissor))
	](Command::Buffer::ExecutionState& a_ExecutionState) {
		const auto& scissorsCount = dynamicScissor ? a_ExecutionState.dynamicStates.scissorsCount : info.scissorsCount;
		const auto& scissorsArray = dynamicScissor ? a_ExecutionState.dynamicStates.scissors : info.scissors;
		for (auto index = 0u; index < scissorsCount; ++index) {
			const auto& scissor = scissorsArray.at(index);
			glScissorIndexed(
				index,
				scissor.offset.x, scissor.offset.y,
				scissor.extent.width, scissor.extent.height
			);
		}
		const auto& viewPortsCount = dynamicViewport ? a_ExecutionState.dynamicStates.viewPortsCount : info.viewPortsCount;
		const auto& viewPortsArray = dynamicViewport ? a_ExecutionState.dynamicStates.viewPorts : info.viewPorts;
		for (auto index = 0u; index < viewPortsCount; ++index) {
			const auto& viewPort = viewPortsArray.at(index);
			glViewportIndexedf(
				index,
				viewPort.rect.offset.x, viewPort.rect.offset.y,
				viewPort.rect.extent.width, viewPort.rect.extent.height
			);
			glDepthRangeIndexed(
				index,
				viewPort.depthRange.near, viewPort.depthRange.far
			);
		}
	};
}
}
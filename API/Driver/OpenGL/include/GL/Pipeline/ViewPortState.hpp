/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:56
*/
#pragma once

#include <OCRA/Core.hpp>

#include <GL/ExecutionState.hpp>
#include <GL/glew.h>

#include <array>

namespace OCRA::Pipeline {
struct CompileViewPortState
{
    CompileViewPortState(const Device::Handle& a_Device, const PipelineViewPortState& a_Info, const PipelineDynamicState& a_DynamicState)
        : dynamicViewport(a_DynamicState.Contains(DynamicState::Viewport))
        , dynamicScissor(a_DynamicState.Contains(DynamicState::Scissor))
        , viewPorts(a_Info.viewPorts)
        , scissors(a_Info.scissors)
    {}
    void operator()(Command::Buffer::ExecutionState& a_ExecutionState) const
    {
        const auto& scissorsVector = dynamicScissor ? a_ExecutionState.dynamicStates.scissors : scissors;
        for (auto index = 0u; index < scissorsVector.size(); ++index) {
            const auto& scissor = scissorsVector.at(index);
            glScissorIndexed(
                index,
                scissor.offset.x, scissor.offset.y,
                scissor.extent.width, scissor.extent.height
            );
        }
        const auto& viewPortsVector = dynamicViewport ? a_ExecutionState.dynamicStates.viewPorts : viewPorts;
        for (auto index = 0u; index < viewPortsVector.size(); ++index) {
            const auto& viewPort = viewPortsVector.at(index);
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
    }
    const bool dynamicViewport;
    const bool dynamicScissor;
    const std::vector<ViewPort> viewPorts;
    const std::vector<Rect2D>   scissors;
};
}
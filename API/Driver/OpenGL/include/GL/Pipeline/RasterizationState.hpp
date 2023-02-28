/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:39
*/
#pragma once

#include <OCRA/Handle.hpp>

#include <GL/Command/ExecutionState.hpp>
#include <GL/Structs.hpp>
#include <GL/Enums.hpp>
#include <GL/glew.h>

namespace OCRA::Pipeline {
struct CompileRasterizationState
{
    CompileRasterizationState(const Device::Handle& a_Device, const PipelineRasterizationState& a_Info, const PipelineDynamicState& a_DynamicState)
        : info(GLRasterizationState(a_Info))
        , dynamicRasterizerDiscardEnable(a_DynamicState.Contains(DynamicState::RasterizerDiscardEnable))
        , dynamicLineWidth(a_DynamicState.Contains(DynamicState::LineWidth))
        , dynamicCullMode(a_DynamicState.Contains(DynamicState::CullMode))
        , dynamicFrontFace(a_DynamicState.Contains(DynamicState::FrontFace))
        , dynamicDepthBiasEnable(a_DynamicState.Contains(DynamicState::DepthBiasEnable))
        , dynamicDepthBias(a_DynamicState.Contains(DynamicState::DepthBias))
    {}
    void operator()(Command::Buffer::ExecutionState& a_ExecutionState) const
    {
        const auto& bRasterizerDiscardEnable = dynamicRasterizerDiscardEnable ? a_ExecutionState.dynamicStates.rasterizerDiscardEnable : info.rasterizerDiscardEnable;
        const auto& bDepthBiasEnable = dynamicDepthBiasEnable ? a_ExecutionState.dynamicStates.depthBiasEnable : info.depthBiasEnable;
        const auto& fDepthBiasConstantFactor = dynamicDepthBias ? a_ExecutionState.dynamicStates.depthBiasConstantFactor : info.depthBiasConstantFactor;
        const auto& fDepthBiasSlopeFactor = dynamicDepthBias ? a_ExecutionState.dynamicStates.depthBiasSlopeFactor : info.depthBiasSlopeFactor;
        const auto& fDepthBiasClamp = dynamicDepthBias ? a_ExecutionState.dynamicStates.depthBiasClamp : info.depthBiasClamp;
        const auto& fLineWidth = dynamicLineWidth ? a_ExecutionState.dynamicStates.lineWidth : info.lineWidth;
        const auto& eFrontFace = dynamicFrontFace ? a_ExecutionState.dynamicStates.frontFace : info.frontFace;
        const auto& eCullMode = dynamicCullMode ? a_ExecutionState.dynamicStates.cullMode : info.cullMode;
        bRasterizerDiscardEnable ? glEnable(GL_RASTERIZER_DISCARD) : glDisable(GL_RASTERIZER_DISCARD);
        info.depthClampEnable ? glEnable(GL_DEPTH_CLAMP) : glDisable(GL_DEPTH_CLAMP);
        bDepthBiasEnable ? glEnable(info.polygonOffsetMode) : glDisable(info.polygonOffsetMode);
        glPolygonMode(
            GL_FRONT_AND_BACK,
            info.polygonMode);
        glPolygonOffsetClamp(
            fDepthBiasConstantFactor,
            fDepthBiasSlopeFactor,
            fDepthBiasClamp);
        glLineWidth(fLineWidth);
        glFrontFace(eFrontFace);
        if (eCullMode != GL_NONE) {
            glEnable(GL_CULL_FACE);
            glCullFace(eCullMode);
        }
        else glDisable(GL_CULL_FACE);
    }
    const GLRasterizationState info;
    const bool dynamicRasterizerDiscardEnable;
    const bool dynamicLineWidth;
    const bool dynamicCullMode;
    const bool dynamicFrontFace;
    const bool dynamicDepthBiasEnable;
    const bool dynamicDepthBias;
};
static inline auto DefaultRasterizationState(const Device::Handle& a_Device)
{
    return CompileRasterizationState(a_Device, {}, {});
}
}
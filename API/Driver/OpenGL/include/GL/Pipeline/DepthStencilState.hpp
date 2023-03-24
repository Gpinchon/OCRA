/*
* @Author: gpinchon
* @Date:   2021-09-27 18:56:42
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-27 22:49:11
*/
#pragma once

#include <OCRA/Core.hpp>

#include <GL/ExecutionState.hpp>
#include <GL/Enums.hpp>
#include <GL/RenderPass.hpp>

#include <GL/glew.h>

namespace OCRA::Pipeline {
struct GLStencilOp {
    GLStencilOp(const GLenum& a_Face, const PipelineStencilOpState& a_OpState, const PipelineDynamicState& a_DynamicState)
        : face(a_Face)
        , dynamicStencilOp(a_DynamicState.Contains(DynamicState::StencilOP))
        , opstate(a_OpState)
    {}
    static inline void Apply(
        const GLenum& a_Face,
        const GLStencilOpState& a_GLOpState,
        const bool a_IgnoreMask)
    {
        glStencilOpSeparate(
            a_Face,
            a_GLOpState.failOp,
            a_GLOpState.depthFailOp,
            a_GLOpState.passOp);
        glStencilFuncSeparate(
            a_Face,
            a_GLOpState.compareOp,
            a_GLOpState.reference,
            a_GLOpState.compareMask);
        if (!a_IgnoreMask) glStencilMaskSeparate(a_Face, a_GLOpState.writeMask);
    }
    void operator()(Command::Buffer::ExecutionState& a_ExecutionState) const {
        const auto& storeOp = a_ExecutionState.renderPass.renderingInfo.stencilAttachment.storeOp;
        const auto ignoreStencilMask = storeOp == StoreOp::DontCare;
        if (dynamicStencilOp) {
            Apply(
                face,
                face == GL_BACK ? a_ExecutionState.dynamicStates.backStencilOP : a_ExecutionState.dynamicStates.frontStencilOP,
                ignoreStencilMask);
        }
        else Apply(face, opstate, ignoreStencilMask);
    }
    const GLenum face;
    const bool dynamicStencilOp;
    const GLStencilOpState opstate;
};
struct CompileDepthStencilState
{
    CompileDepthStencilState(const Device::Handle& a_Device, const PipelineDepthStencilState& a_Info, const PipelineDynamicState& a_DynamicState)
        : depthTestEnable(a_Info.depthTestEnable)
        , depthWriteEnable(a_Info.depthWriteEnable)
        , depthCompareOp(GetGLOperation(a_Info.depthCompareOp))
        , depthBoundsTestEnable(a_Info.depthBoundsTestEnable)
        , stencilTestEnable(a_Info.stencilTestEnable)
        , front(GLStencilOp(GL_FRONT, a_Info.frontStencilOpState, a_DynamicState))
        , back(GLStencilOp(GL_BACK, a_Info.backStencilOpState, a_DynamicState))
        , depthBounds(a_Info.depthBounds)
        , dynamicDepthTestEnable(a_DynamicState.Contains(DynamicState::DepthTestEnable))
        , dynamicDepthWriteEnable(a_DynamicState.Contains(DynamicState::DepthWriteEnable))
        , dynamicDepthCompareOP(a_DynamicState.Contains(DynamicState::DepthCompareOP))
        , dynamicDepthBoundsTestEnable(a_DynamicState.Contains(DynamicState::DepthBoundsTestEnable))
        , dynamicStencilTestEnable(a_DynamicState.Contains(DynamicState::StencilTestEnable))
        , dynamicDepthBounds(a_DynamicState.Contains(DynamicState::DepthBounds))
    {}
    void operator()(Command::Buffer::ExecutionState& a_ExecutionState) const
    {
        const auto bDepthTestEnable = dynamicDepthTestEnable ? a_ExecutionState.dynamicStates.depthTestEnable : depthTestEnable;
        const auto bDepthWriteEnable = dynamicDepthWriteEnable ? a_ExecutionState.dynamicStates.depthWriteEnable : depthWriteEnable;
        const auto bDepthBoundsTestEnable = dynamicDepthBoundsTestEnable ? a_ExecutionState.dynamicStates.depthBoundsTestEnable : depthBoundsTestEnable;
        const auto sDepthBounds = dynamicDepthBounds ? a_ExecutionState.dynamicStates.depthBounds : depthBounds;
        const auto eDepthCompareOp = dynamicDepthCompareOP ? a_ExecutionState.dynamicStates.depthCompareOp : depthCompareOp;
        const auto bStencilTestEnable = dynamicStencilTestEnable ? a_ExecutionState.dynamicStates.stencilTestEnable : stencilTestEnable;
        bDepthTestEnable ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
        bDepthBoundsTestEnable ? glEnable(GL_DEPTH_BOUNDS_TEST_EXT) : glDisable(GL_DEPTH_BOUNDS_TEST_EXT);
        const auto& storeOp = a_ExecutionState.renderPass.renderingInfo.depthAttachment.storeOp;
        const auto ignoreDepthMask = storeOp == StoreOp::DontCare;
        if (!ignoreDepthMask) glDepthMask(bDepthWriteEnable);
        glDepthFunc(eDepthCompareOp);
        bStencilTestEnable  ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
        front(a_ExecutionState);
        back(a_ExecutionState);
        glDepthBoundsEXT(
            sDepthBounds.min,
            sDepthBounds.max);
    }
    const bool   depthTestEnable;
    const bool   depthWriteEnable;
    const bool   depthBoundsTestEnable;
    const bool   stencilTestEnable;
    const GLenum depthCompareOp;
    const GLStencilOp front;
    const GLStencilOp back;
    const DepthBounds<double> depthBounds;
    const bool dynamicDepthTestEnable;
    const bool dynamicDepthWriteEnable;
    const bool dynamicDepthCompareOP;
    const bool dynamicDepthBoundsTestEnable;
    const bool dynamicStencilTestEnable;
    const bool dynamicDepthBounds;
};
inline auto DefaultDepthStencilState(const Device::Handle& a_Device)
{
    return CompileDepthStencilState(a_Device, {}, {});
}
}
/*
* @Author: gpinchon
* @Date:   2021-09-27 18:56:42
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-27 22:49:11
*/
#pragma once

#include <OCRA/Handle.hpp>
#include <OCRA/Pipeline/DynamicState.hpp>
#include <OCRA/Pipeline/DepthStencilState.hpp>

#include <GL/Command/ExecutionState.hpp>
#include <GL/Common/Compare.hpp>
#include <GL/Common/Stencil.hpp>
#include <GL/RenderPass.hpp>
#include <GL/glew.h>

#include <functional>

namespace OCRA::Pipeline::DepthStencilState {
struct GLStencilOp {
    GLStencilOp(const GLenum& a_Face, const Stencil::OpState& a_OpState, const DynamicState::Info& a_DynamicState)
        : face(a_Face)
        , dynamicStencilOp(a_DynamicState.Contains(DynamicState::State::StencilOP))
        , opstate(a_OpState)
    {}
    static inline void Apply(
        const GLenum& a_Face,
        const Stencil::GLOpState& a_GLOpState,
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
        const auto& renderPass = a_ExecutionState.renderPass.renderPass;
        const auto ignoreStencilMask = renderPass != nullptr && renderPass->info.depthAttachment.storeOp == RenderPass::StoreOperation::DontCare;
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
    const Stencil::GLOpState opstate;
};
struct Compile
{
    Compile(const Device::Handle& a_Device, const DepthStencilState::Info& a_Info, const DynamicState::Info& a_DynamicState)
        : depthTestEnable(a_Info.depthTestEnable)
        , depthWriteEnable(a_Info.depthWriteEnable)
        , depthCompareOp(GetGLOperation(a_Info.depthCompareOp))
        , depthBoundsTestEnable(a_Info.depthBoundsTestEnable)
        , stencilTestEnable(a_Info.stencilTestEnable)
        , front(GLStencilOp(GL_FRONT, a_Info.frontStencilOpState, a_DynamicState))
        , back(GLStencilOp(GL_BACK, a_Info.backStencilOpState, a_DynamicState))
        , depthBounds(a_Info.depthBounds)
        , dynamicDepthTestEnable(a_DynamicState.Contains(DynamicState::State::DepthTestEnable))
        , dynamicDepthWriteEnable(a_DynamicState.Contains(DynamicState::State::DepthWriteEnable))
        , dynamicDepthCompareOP(a_DynamicState.Contains(DynamicState::State::DepthCompareOP))
        , dynamicDepthBoundsTestEnable(a_DynamicState.Contains(DynamicState::State::DepthBoundsTestEnable))
        , dynamicStencilTestEnable(a_DynamicState.Contains(DynamicState::State::StencilTestEnable))
        , dynamicDepthBounds(a_DynamicState.Contains(DynamicState::State::DepthBounds))
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
        const auto& renderPass = a_ExecutionState.renderPass.renderPass;
        const auto ignoreDepthMask = renderPass != nullptr && renderPass->info.depthAttachment.storeOp == RenderPass::StoreOperation::DontCare;
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
inline auto Default(const Device::Handle& a_Device)
{
    return Compile(a_Device, {}, {});
}
}
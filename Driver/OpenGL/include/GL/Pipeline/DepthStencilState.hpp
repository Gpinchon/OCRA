/*
* @Author: gpinchon
* @Date:   2021-09-27 18:56:42
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-27 22:49:11
*/
#pragma once

#include <Handle.hpp>
#include <Pipeline/DepthStencilState.hpp>

#include <GL/Command/Buffer/ExecutionState.hpp>
#include <GL/Compare.hpp>
#include <GL/Stencil.hpp>

#include <GL/glew.h>

namespace OCRA::Pipeline::DepthStencilState {
inline void ApplyStencilOP(
    const GLenum& face,
    const GLenum& failOp, const GLenum& passOp, const GLenum& depthFailOp, const GLenum& compareOp,
    const GLuint& compareMask, const GLuint& writeMask, const GLuint& reference
) {
    glStencilOpSeparate(
        face,
        failOp,
        depthFailOp,
        passOp);
    glStencilFuncSeparate(
        face,
        compareOp,
        reference,
        compareMask);
    glStencilMaskSeparate(
        face,
        writeMask);
};
inline void ApplyStencilOP(const GLenum& face, const Stencil::OpState& stencilOP) {
    const auto failOp(GetGLOperation(stencilOP.failOp));
    const auto passOp(GetGLOperation(stencilOP.passOp));
    const auto depthFailOp(GetGLOperation(stencilOP.depthFailOp));
    const auto compareOp(GetGLOperation(stencilOP.compareOp));
    ApplyStencilOP(
        face,
        failOp, passOp, depthFailOp, compareOp,
        stencilOP.compareMask, stencilOP.writeMask, stencilOP.reference);
};
inline const auto CompileStencilOp(const GLenum& face, const StencilOpState& a_StencilOpState, const DynamicState::Info& a_DynamicState)
{
    if (a_DynamicState.Contains(DynamicState::State::StencilOP)) {
        return [](Command::Buffer::ExecutionState& a_ExecutionState) {
            ApplyStencilOP(
                face,
                face == GL_BACK ? a_ExecutionState.dynamicStates.backStencilOP : a_ExecutionState.dynamicStates.frontStencilOP
            );
        };
    }
    else return [
        failOp(GetGLOperation(a_StencilOpState.failOp)),
        passOp(GetGLOperation(a_StencilOpState.passOp)),
        depthFailOp(GetGLOperation(a_StencilOpState.depthFailOp)),
        compareOp(GetGLOperation(a_StencilOpState.compareOp)),
        compareMask(a_StencilOpState.compareMask),
        writeMask(a_StencilOpState.writeMask),
        reference(a_StencilOpState.reference)
    ](Command::Buffer::ExecutionState&) {
        ApplyStencilOP(
            face,
            failOp, passOp, depthFailOp, compareOp,
            compareMask, writeMask, reference
        );
    };
}
inline const auto Compile(const Device::Handle& a_Device, const DepthStencilState::Info& a_Info, const DynamicState::Info& a_DynamicState)
{
    const auto depthTestEnable(a_Info.depthTestEnable),;
    const auto depthWriteEnable(a_Info.depthWriteEnable),;
    const auto depthCompareOp(GetGLOperation(a_Info.depthCompareOp)),;
    const auto depthBoundsTestEnable(a_Info.depthBoundsTestEnable),;
    const auto stencilTestEnable(a_Info.stencilTestEnable),;
    const auto front = CompileStencilOp(GL_FRONT, a_Info.frontStencilOpState, a_DynamicState),;
    const auto back = CompileStencilOp(GL_BACK, a_Info.backStencilOpState, a_DynamicState),;
    const auto depthBounds(a_Info.depthBounds),;
    const auto dynamicDepthTestEnable(a_DynamicState.Contains(DynamicState::State::DepthTestEnable)),;
    const auto dynamicDepthWriteEnable(a_DynamicState.Contains(DynamicState::State::DepthWriteEnable)),;
    const auto dynamicDepthCompareOP(a_DynamicState.Contains(DynamicState::State::DepthCompareOP)),;
    const auto dynamicDepthBoundsTestEnable(a_DynamicState.Contains(DynamicState::State::DepthBoundsTestEnable)),;
    const auto dynamicStencilTestEnable(a_DynamicState.Contains(DynamicState::State::StencilTestEnable)),;
    const auto dynamicDepthBounds(a_DynamicState.Contains(DynamicState::State::DepthBounds));
    return [=](Command::Buffer::ExecutionState& a_ExecutionState) {
        const auto bDepthTestEnable{ dynamicDepthTestEnable ? a_ExecutionState.dynamicStates.depthTestEnable : depthTestEnable };
        const auto bDepthWriteEnable{ dynamicDepthWriteEnable ? a_ExecutionState.dynamicStates.depthWriteEnable : depthWriteEnable };
        const auto bDepthBoundsTestEnable{ dynamicDepthBoundsTestEnable ? a_ExecutionState.dynamicStates.depthBoundsTestEnable : depthBoundsTestEnable };
        const auto sDepthBounds{ dynamicDepthBounds ? a_ExecutionState.dynamicStates.depthBounds : depthBounds };
        const auto eDepthCompareOp{ dynamicDepthCompareOP ? GetGLOperation(a_ExecutionState.dynamicStates.depthCompareOp) : depthCompareOp };
        const auto bStencilTestEnable{ dynamicStencilTestEnable ? a_ExecutionState.dynamicStates.stencilTestEnable : stencilTestEnable };
        bDepthTestEnable ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
        bDepthBoundsTestEnable ? glEnable(GL_DEPTH_BOUNDS_TEST_EXT) : glDisable(GL_DEPTH_BOUNDS_TEST_EXT);
        glDepthMask(bDepthWriteEnable);
        glDepthFunc(eDepthCompareOp);
        bStencilTestEnable  ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
        front(a_ExecutionState);
        back(a_ExecutionState);
        //extension : GL_EXT_depth_bounds_test
        glDepthBoundsEXT(
            sDepthBounds.min,
            sDepthBounds.max);
    };
}
inline auto Default(const Device::Handle& a_Device)
{
    return Compile(a_Device, {}, {});
}
}
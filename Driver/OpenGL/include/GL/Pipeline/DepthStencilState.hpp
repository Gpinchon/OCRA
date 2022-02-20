/*
* @Author: gpinchon
* @Date:   2021-09-27 18:56:42
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-27 22:49:11
*/
#pragma once

#include <Handle.hpp>
#include <Pipeline/DepthStencilState.hpp>

#include <GL/Compare.hpp>
#include <GL/Stencil.hpp>

#include <GL/glew.h>

namespace OCRA::Pipeline::DepthStencilState {
inline const auto Compile(const StencilOpState& a_StencilOpState, GLenum a_Face)
{
    return [
        failOp(GetGLOperation(a_StencilOpState.failOp)),
        passOp(GetGLOperation(a_StencilOpState.passOp)),
        depthFailOp(GetGLOperation(a_StencilOpState.depthFailOp)),
        compareOp(GetGLOperation(a_StencilOpState.compareOp)),
        compareMask(a_StencilOpState.compareMask),
        writeMask(a_StencilOpState.writeMask),
        reference(a_StencilOpState.reference),
        face(a_Face)
    ]() {
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
}
inline const auto Compile(const Device::Handle& a_Device, const DepthStencilState::Info& a_Info)
{
    return [
        depthTestEnable(a_Info.depthTestEnable),
        depthWriteEnable(a_Info.depthWriteEnable),
        depthCompareOp(GetGLOperation(a_Info.depthCompareOp)),
        depthBoundsTestEnable(a_Info.depthBoundsTestEnable),
        stencilTestEnable(a_Info.stencilTestEnable),
        front = Compile(a_Info.front, GL_FRONT),
        back = Compile(a_Info.back, GL_BACK),
        minDepthBounds(a_Info.minDepthBounds),
        maxDepthBounds(a_Info.maxDepthBounds)

    ]() {
        depthTestEnable ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
        glDepthMask(depthWriteEnable);
        glDepthFunc(depthCompareOp);
        stencilTestEnable ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
        front();
        back();
        //extension : GL_EXT_depth_bounds_test
        depthBoundsTestEnable ? glEnable(GL_DEPTH_BOUNDS_TEST_EXT) : glDisable(GL_DEPTH_BOUNDS_TEST_EXT);
        glDepthBoundsEXT(
            minDepthBounds,
            maxDepthBounds);
    };
}
inline auto Default(const Device::Handle& a_Device)
{
    return Compile(a_Device, {});
}
}
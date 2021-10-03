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
struct Compile {
    struct StencilOpState {
        StencilOpState(const DepthStencilState::StencilOpState& a_StencilOpState)
            : failOp(GetGLOperation(a_StencilOpState.failOp))
            , passOp(GetGLOperation(a_StencilOpState.passOp))
            , depthFailOp(GetGLOperation(a_StencilOpState.depthFailOp))
            , compareOp(GetGLOperation(a_StencilOpState.compareOp))
            , compareMask(a_StencilOpState.compareMask)
            , writeMask(a_StencilOpState.writeMask)
            , reference(a_StencilOpState.reference)
        {
        }
        const GLenum failOp; //the operation to be realized when stencil test FAILS
        const GLenum passOp; //the operation to be realized when stencil and depth test PASSES
        const GLenum depthFailOp; //the operation to be realized when stencil test PASSES but depth test FAILS
        const GLenum compareOp;
        const GLuint compareMask; //a mask that is ANDed with ref and the buffer's content
        const GLuint writeMask; //a mask that is ANDed with the stencil value about to be written to the buffer
        const GLuint reference; //the reference value used in comparison.
        void operator()(const GLenum& a_Face) const noexcept
        {
            glStencilOpSeparate(
                a_Face,
                failOp,
                depthFailOp,
                passOp);
            glStencilFuncSeparate(
                a_Face,
                compareOp,
                reference,
                compareMask);
            glStencilMaskSeparate(
                a_Face,
                writeMask);
        }
    };
    Compile(const Device::Handle& a_Device, const DepthStencilState::Info& a_Info)
        : depthTestEnable(a_Info.depthTestEnable)
        , depthWriteEnable(a_Info.depthWriteEnable)
        , depthCompareOp(GetGLOperation(a_Info.depthCompareOp))
        , depthBoundsTestEnable(a_Info.depthBoundsTestEnable)
        , stencilTestEnable(a_Info.stencilTestEnable)
        , front(a_Info.front)
        , back(a_Info.back)
        , minDepthBounds(a_Info.minDepthBounds)
        , maxDepthBounds(a_Info.maxDepthBounds)
    {
    }
    const GLboolean depthTestEnable;
    const GLboolean depthWriteEnable;
    const GLenum depthCompareOp;
    const GLboolean depthBoundsTestEnable;
    const GLboolean stencilTestEnable;
    const StencilOpState front;
    const StencilOpState back;
    const GLfloat minDepthBounds;
    const GLfloat maxDepthBounds;
    void operator()(const Device::Handle& a_Device) const
    {
        glDepthMask(depthWriteEnable);
        glDepthFunc(depthCompareOp);
        depthTestEnable ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
        stencilTestEnable ? glEnable(GL_STENCIL_TEST) : glDisable(GL_STENCIL_TEST);
        front(GL_FRONT);
        back(GL_BACK);
		//extension : GL_EXT_depth_bounds_test
        depthBoundsTestEnable ? glEnable(GL_DEPTH_BOUNDS_TEST_EXT) : glDisable(GL_DEPTH_BOUNDS_TEST_EXT);
        glDepthBoundsEXT(
            minDepthBounds,
            maxDepthBounds);
    }
};
inline auto Default(const Device::Handle& a_Device)
{
    return Compile(a_Device, {});
}
}
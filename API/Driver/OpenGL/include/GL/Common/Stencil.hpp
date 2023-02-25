/*
* @Author: gpinchon
* @Date:   2021-09-27 19:02:59
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-27 19:03:06
*/
#pragma once

#include <OCRA/Enums.hpp>

#include <GL/Common/Compare.hpp>
#include <GL/glew.h>

#include <stdexcept>

namespace OCRA {
static inline auto GetGLOperation(const StencilOp& a_Operation) {
    switch (a_Operation)
    {
    case StencilOp::Keep:
        return GL_KEEP;
    case StencilOp::Zero:
        return GL_ZERO;
    case StencilOp::Replace:
        return GL_REPLACE;
    case StencilOp::IncrementAndClamp:
        return GL_INCR;
    case StencilOp::DecrementAndClamp:
        return GL_DECR;
    case StencilOp::Invert:
        return GL_INVERT;
    case StencilOp::IncrementAndWrap:
        return GL_INCR_WRAP;
    case StencilOp::DecrementAndWrap:
        return GL_DECR_WRAP;
    default:
        throw std::runtime_error("Unknown Stencil Operation");
    }
}
}

namespace OCRA::Stencil {

struct GLOpState {
    GLOpState(const StencilOpState& a_OpState = {})
    : failOp(GetGLOperation(a_OpState.failOp))
    , passOp(GetGLOperation(a_OpState.passOp))
    , depthFailOp(GetGLOperation(a_OpState.depthFailOp))
    , compareOp(GetGLOperation(a_OpState.compareOp))
    , compareMask(a_OpState.compareMask)
    , writeMask(a_OpState.writeMask)
    , reference(a_OpState.reference)
    {}
    GLenum failOp; //the operation to be realized when stencil test FAILS
    GLenum passOp; //the operation to be realized when stencil test PASSES
    GLenum depthFailOp; //the operation to be realized when stencil test PASSES but depth test FAILS
    GLenum compareOp;
    uint32_t compareMask; //a mask that is ANDed with ref and the buffer's content
    uint32_t writeMask; //a mask that is ANDed with the stencil value about to be written to the buffer
    uint32_t reference; //the reference value used in comparison.
};
}
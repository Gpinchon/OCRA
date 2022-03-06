/*
* @Author: gpinchon
* @Date:   2021-09-27 19:02:59
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-27 19:03:06
*/
#pragma once

#include <Stencil.hpp>

#include <GL/Compare.hpp>
#include <GL/glew.h>

#include <stdexcept>

namespace OCRA::Stencil {
static inline auto GetGLOperation(const Operation& a_Operation) {
	switch (a_Operation)
	{
	case Operation::Keep:
		return GL_KEEP;
	case Operation::Zero:
		return GL_ZERO;
	case Operation::Replace:
		return GL_REPLACE;
	case Operation::IncrementAndClamp:
		return GL_INCR;
	case Operation::DecrementAndClamp:
		return GL_DECR;
	case Operation::Invert:
		return GL_INVERT;
	case Operation::IncrementAndWrap:
		return GL_INCR_WRAP;
	case Operation::DecrementAndWrap:
		return GL_DECR_WRAP;
	default:
		throw std::runtime_error("Unknown Stencil Operation");
	}
}
struct GLOpState {
	GLOpState(const OpState& a_OpState = {})
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
    Uint32 compareMask; //a mask that is ANDed with ref and the buffer's content
    Uint32 writeMask; //a mask that is ANDed with the stencil value about to be written to the buffer
    Uint32 reference; //the reference value used in comparison.
};
}
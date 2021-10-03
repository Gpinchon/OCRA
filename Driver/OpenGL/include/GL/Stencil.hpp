/*
* @Author: gpinchon
* @Date:   2021-09-27 19:02:59
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-27 19:03:06
*/
#pragma once

#include <Stencil.hpp>

#include <stdexcept>

#include <GL/glew.h>

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
}
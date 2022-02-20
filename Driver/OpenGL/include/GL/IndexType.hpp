/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-03 22:11:23
*/
#pragma once

#include <IndexType.hpp>

#include <GL/glew.h>

#include <stdexcept>

namespace OCRA
{
static inline GLenum GetGLIndexType(const IndexType& a_IndexType)
{
	switch (a_IndexType)
	{
	case IndexType::None:
		return GL_NONE;
	case IndexType::Uint32:
		return GL_UNSIGNED_INT;
	case IndexType::Uint16:
		return GL_UNSIGNED_SHORT;
	case IndexType::Uint8:
		return GL_UNSIGNED_BYTE;
	default:
		throw std::runtime_error("Unknown Index Type");
	}
}
}
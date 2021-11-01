/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-03 22:11:23
*/
#pragma once

#include <VertexType.hpp>

#include <GL/glew.h>

#include <stdexcept>

namespace OCRA
{
static inline auto GetGLVertexType(const VertexType& a_VertexType)
{
	switch (a_VertexType)
	{
	case OCRA::VertexType::None:
		return GL_NONE;
	case OCRA::VertexType::Float32:
		return GL_FLOAT;
	case OCRA::VertexType::Float16:
		return GL_HALF_FLOAT;
	case OCRA::VertexType::Int32:
		return GL_INT;
	case OCRA::VertexType::Uint32:
		return GL_UNSIGNED_INT;
	case OCRA::VertexType::Int16:
		return GL_SHORT;
	case OCRA::VertexType::Uint16:
		return GL_UNSIGNED_SHORT;
	case OCRA::VertexType::Int8:
		return GL_BYTE;
	case OCRA::VertexType::Uint8:
		return GL_UNSIGNED_BYTE;
	default:
		throw std::runtime_error("Unknown Vertex Type");
	}
}
}
/*
* @Author: gpinchon
* @Date:   2021-10-04 20:24:27
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-04 20:25:30
*/
#pragma once

#include <Handle.hpp>
#include <Shader/Stage.hpp>

#include <stdexcept>

#include <GL/glew.h>

namespace OCRA::Shader::Stage {
static inline auto GetGLStage(const Stage& a_Stage)
{
	switch (a_Stage)
	{
	case Stage::Vertex:
		return GL_VERTEX_SHADER;
	case Stage::Geometry:
		return GL_GEOMETRY_SHADER;
	case Stage::Fragment:
		return GL_FRAGMENT_SHADER;
	case Stage::Compute:
		return GL_COMPUTE_SHADER;
	case Stage::TessControl:
		return GL_TESS_CONTROL_SHADER;
	case Stage::TessEval:
		return GL_TESS_EVALUATION_SHADER;
	default:
		throw std::runtime_error("Unknown Shader Type");
	}
}
unsigned GetGLHandle(const Device::Handle& a_Device, const Handle& a_Handle);
}
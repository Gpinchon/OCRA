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
static inline auto GetGLStage(const StageFlags& a_Stage)
{
	switch (a_Stage)
	{
	case StageFlagBits::Vertex:
		return GL_VERTEX_SHADER;
	case StageFlagBits::Geometry:
		return GL_GEOMETRY_SHADER;
	case StageFlagBits::Fragment:
		return GL_FRAGMENT_SHADER;
	case StageFlagBits::Compute:
		return GL_COMPUTE_SHADER;
	case StageFlagBits::TessControl:
		return GL_TESS_CONTROL_SHADER;
	case StageFlagBits::TessEval:
		return GL_TESS_EVALUATION_SHADER;
	default:
		throw std::runtime_error("Unknown Shader Stage");
	}
}
unsigned GetGLHandle(const Handle& a_Handle);
}
/*
* @Author: gpinchon
* @Date:   2021-10-04 20:24:27
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-04 20:25:30
*/
#pragma once

#include <Handle.hpp>
#include <Shader/Stage.hpp>

#include <GL/glew.h>

#include <stdexcept>

namespace OCRA::Shader::Stage {
static inline auto GetGLStage(const StageFlags& a_Stage)
{
	if (a_Stage == StageFlagBits::Vertex)
		return GL_VERTEX_SHADER;
	if (a_Stage == StageFlagBits::Geometry)
		return GL_GEOMETRY_SHADER;
	if (a_Stage == StageFlagBits::Fragment)
		return GL_FRAGMENT_SHADER;
	if (a_Stage == StageFlagBits::Compute)
		return GL_COMPUTE_SHADER;
	if (a_Stage == StageFlagBits::TessControl)
		return GL_TESS_CONTROL_SHADER;
	if (a_Stage == StageFlagBits::TessEval)
		return GL_TESS_EVALUATION_SHADER;
	throw std::runtime_error("Unknown Shader Stage");
}
unsigned GetGLHandle(const Handle& a_Handle);
}
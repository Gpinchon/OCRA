/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 15:03:34
*/
#pragma once

#include <Handle.hpp>
#include <Pipeline/ShaderStageState.hpp>
#include <Shader/Stage.hpp>

#include <stdexcept>
#include <vector>

#include <GL/glew.h>
#include <GL/Shader/Stage.hpp>

namespace OCRA::Pipeline::ShaderStageState {
//compiles the specified stages into a program
struct Compile {
	Compile(const Device::Handle& a_Device, const Info& a_Info)
	{
        glGenProgramPipelines(1, &handle);
		std::vector<GLuint> stages;
		for (const auto& stageHandle : a_Info.stages) {
			const auto& stageInfo{ Shader::Stage::GetInfo(a_Device, stageHandle) };
			const auto& stageGLHandle{ Shader::Stage::GetGLHandle(a_Device, stageHandle) };
            glUseProgramStages(handle, Shader::Stage::GetGLStage(stageInfo.stage), stageGLHandle);
		}
	}
	~Compile()
	{
		glDeleteProgramPipelines(1, &handle);
	}
	void operator()(const Device::Handle& a_Device)
	{
		glBindProgramPipeline(handle);
	}
	GLuint handle;
};
}
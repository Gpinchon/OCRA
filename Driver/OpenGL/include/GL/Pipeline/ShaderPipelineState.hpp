/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 15:03:34
*/
#pragma once

#include <Pipeline/ShaderPipelineState.hpp>

#include <GL/Shader/Stage.hpp>
#include <GL/glew.h>

OCRA_DECLARE_HANDLE(OCRA::Device);

namespace OCRA::Pipeline::DynamicState {
struct Info;
}

namespace OCRA::Command::Buffer {
struct ExecutionState;
}

namespace OCRA::Pipeline::ShaderPipelineState {
//compiles the specified stages into a program pipeline
inline auto Compile(const Device::Handle& a_Device, const Info& a_Info, const DynamicState::Info&)
{
	GLuint handle;
	glGenProgramPipelines(1, &handle);
	for (const auto& stage : a_Info.stages) {
		const auto& stageInfo{ Shader::Stage::GetInfo(stage) };
		const auto& stageGLHandle{ Shader::Stage::GetGLHandle(stage) };
		glUseProgramStages(handle, Shader::Stage::GetGLStage(stageInfo.stage), stageGLHandle);
	}
	return [handle](Command::Buffer::ExecutionState&){
		glBindProgramPipeline(handle);
	};
}
}
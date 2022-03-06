/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 15:03:34
*/
#pragma once

#include <Handle.hpp>

#include <GL/Pipeline/ShaderPipelinePool.hpp>
#include <GL/glew.h>

namespace OCRA::Pipeline::DynamicState {
struct Info;
}

namespace OCRA::Command::Buffer {
struct ExecutionState;
}

namespace OCRA::Pipeline::ShaderPipelineState {
//compiles the specified stages into a program
struct Compile {
	Compile(const Device::Handle& a_Device, const Info& a_Info, const DynamicState::Info&);
	void operator()(Command::Buffer::ExecutionState&) const noexcept {
		glBindProgramPipeline(shaderPipelineRef->handle);
	}
	ShaderPipelinePool::Reference shaderPipelineRef;
};
}
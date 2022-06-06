/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 15:03:34
*/
#pragma once

#include <Pipeline/ShaderPipelineState.hpp>

#include <GL/WeakHandle.hpp>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Pipeline::DynamicState {
struct Info;
}

namespace OCRA::Command::Buffer {
struct ExecutionState;
}

namespace OCRA::Pipeline::ShaderPipelineState {
//compiles the specified stages into a program pipeline
struct Compile {
	Compile(const Device::Handle& a_Device, const Info& a_Info, const DynamicState::Info&);
	Compile(const Compile& a_Other);
	~Compile();
	void operator()(Command::Buffer::ExecutionState&) const;
	const Device::WeakHandle device;
	const Info& info;
	mutable uint32_t handle{ 0 };
	mutable uint32_t pushConstantHandle{ 0 };
};
}
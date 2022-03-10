/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:45
*/
#pragma once

#include <Handle.hpp>
#include <Shader/Stage.hpp>

#include <array>

HANDLE(OCRA::Pipeline::ShaderPipelineState);

namespace OCRA::Pipeline::ShaderPipelineState {
constexpr auto MaxStages = 6;
struct Info {
	uint8_t stageCount{ 0 };
	std::array<Shader::Stage::Handle, MaxStages> stages{ 0 };
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info);
}
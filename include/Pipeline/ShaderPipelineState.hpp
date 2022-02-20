/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:45
*/
#pragma once

#include <Handle.hpp>
#include <Scalar.hpp>

#include <array>

namespace OCRA::Pipeline::ShaderPipelineState {
constexpr auto MaxStages = 6;
struct Info {
	Uint8 stageCount{ 0 };
	std::array<Shader::Stage::Handle, MaxStages> stages{ 0 };
};
}
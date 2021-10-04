/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:45
*/
#pragma once

#include <Handle.hpp>
#include <Scalar.hpp>

#include <vector>

namespace OCRA::Pipeline::ShaderStageState {
struct Info {
    std::vector<Shader::Stage::Handle> stages;
};
}
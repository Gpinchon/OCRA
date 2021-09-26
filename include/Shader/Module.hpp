/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:08
*/
#pragma once

#include <Scalar.hpp>

#include <vector>

namespace OCRA::Shader::Module {
struct Info {
    std::vector<Uint8> code;
};
}
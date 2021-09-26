/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:45
*/
#pragma once

#include <Handle.hpp>
#include <Scalar.hpp>

namespace OCRA::Pipeline::ShaderStage {
typedef std::bitset<6> StageFlag;
constexpr StageFlag None = 0b000000;
constexpr StageFlag Vertex = 0b100000;
constexpr StageFlag Geometry = 0b010000;
constexpr StageFlag Fragment = 0b001000;
constexpr StageFlag Compute = 0b000100;
constexpr StageFlag TessControl = 0b000010;
constexpr StageFlag TessEval = 0b000001;
constexpr StageFlag AllGraphics = 0b111000; //Vertex | Geometry | Fragment
constexpr StageFlag All = 0b111111;
struct Info {
    StageFlag stage { None };
    Shader::Module::Handle module;
    std::string name;
};
}
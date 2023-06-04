#pragma once

#include <OCRA/ShaderCompiler/Enums.hpp>

#include <string>

namespace OCRA::ShaderCompiler {
struct ShaderInfo {
    TargetAPI targetAPI;
    ShaderType type { ShaderType::Unknown }; // the shader's type (Vertex, Geometry, Fragment...)
    std::string entryPoint {}; // the shader's entry point
    std::string source {}; // the shader's human readable source code
};
}

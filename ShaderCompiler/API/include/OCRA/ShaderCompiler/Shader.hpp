#pragma once

#include <OCRA/ShaderCompiler/Handle.hpp>

#include <string>
#include <vector>

OCRA_DECLARE_HANDLE(OCRA::ShaderCompiler);
OCRA_DECLARE_HANDLE(OCRA::ShaderCompiler::Shader);

namespace OCRA::ShaderCompiler::Shader
{
enum class Type {
    Unknown = -1,
    Vertex, Geometry, Fragment,
    Compute,
    TessControl, TessEval,
    MaxValue
};

struct Info {
    Type        type{ Type::Unknown }; //the shader's type (Vertex, Geometry, Fragment...)
    std::string entryPoint{};          //the shader's entry point
    std::string source{};              //the shader's human readable source code
};
/**
* @brief The shader language is vulkan glsl.
* Multiple descriptor sets aren't supported,
* either use set=0 or no set at all.
*/
Handle Create(const ShaderCompiler::Handle& a_Compiler, const Info& a_Info);
std::vector<uint32_t> Compile(const Handle& a_Shader);
}
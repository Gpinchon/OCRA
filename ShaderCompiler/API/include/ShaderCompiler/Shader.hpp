#pragma once

#include <string>
#include <vector>
#include <ShaderCompiler/Handle.hpp>

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
/**
* @arg type : the shader's type (Vertex, Geometry, Fragment...)
* @arg entryPoint : the shader's entry point
* @arg source : the shader's human readable source code
*/
struct Info {
    Type        type{ Type::Unknown };
    std::string entryPoint{};
    std::string source{};
};
Handle Create(const ShaderCompiler::Handle& a_Compiler, const Info& a_Info);
std::vector<uint32_t> Compile(const Handle& a_Shader);
}
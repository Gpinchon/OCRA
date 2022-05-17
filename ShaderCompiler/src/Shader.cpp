#include <Shader.hpp>

#include <shaderc/shaderc.hpp>

#include <stdexcept>

namespace OCRA::ShaderCompiler
{
const auto inline GetShaderCType(const Shader::Type& a_Type)
{
    switch(a_Type) {
        case Shader::Type::Vertex :
            return shaderc_vertex_shader;
        case Shader::Type::Geometry :
            return shaderc_geometry_shader;
        case Shader::Type::Fragment :
            return shaderc_fragment_shader;
        case Shader::Type::Compute :
            return shaderc_compute_shader;
        case Shader::Type::TessControl :
            return shaderc_tess_control_shader;
        case Shader::Type::TessEval :
            return shaderc_tess_evaluation_shader;
    }
    throw std::runtime_error("Unknown shader type !");
}
std::vector<uint32_t> Shader::Compile(bool a_Optimize)
{
    shaderc::Compiler        compiler;
    shaderc::CompileOptions  compilerOptions;
    if (a_Optimize) compilerOptions.SetOptimizationLevel(shaderc_optimization_level_size);
    const auto module = compiler.CompileGlslToSpv(source, GetShaderCType(type), name.c_str(), compilerOptions);
    if (module.GetCompilationStatus() != shaderc_compilation_status_success)
        throw std::runtime_error(module.GetErrorMessage());
    return { module.cbegin(), module.cend() };
}
}
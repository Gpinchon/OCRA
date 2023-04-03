#include <OCRA/ShaderCompiler/Shader.hpp>

#include <glslang\Public\ShaderLang.h>
#include <glslang\Public\ResourceLimits.h>
#include <SPIRV\GlslangToSpv.h>
#include <spirv_cross.hpp>
#include <spirv_glsl.hpp>

#include <stdexcept>
#include <iostream>

namespace OCRA::ShaderCompiler::Shader
{
struct Impl
{
    Impl(const ShaderCompiler::Handle& a_Compiler, const Info& a_Info)
        : info(a_Info)
    {}
    //compile to SPIRV binary words
    std::vector<uint32_t> Compile();
    const Info info;
};

std::vector<uint32_t> Impl::Compile()
{
    
}
Handle Create(const ShaderCompiler::Handle& a_Compiler, const Info& a_Info) {
    return Handle(new Impl(a_Compiler, a_Info));
}
std::vector<uint32_t> Compile(const Handle& a_Shader) {
    return a_Shader->Compile();
}
}
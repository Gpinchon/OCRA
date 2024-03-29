#include <OCRA/ShaderCompiler/Compiler.hpp>

#include <OCRA/ShaderCompiler/Impl/Compiler.hpp>

namespace OCRA::ShaderCompiler {
Handle OCRA::ShaderCompiler::Create()
{
    return Handle(new Impl);
}
CompiledShader Compile(const Handle& a_Compiler, const ShaderInfo& a_Info)
{
    return a_Compiler->Compile(a_Info);
}
}

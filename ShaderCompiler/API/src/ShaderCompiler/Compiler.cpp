#include <ShaderCompiler/Compiler.hpp>

#include <glslang\Public\ShaderLang.h>

namespace OCRA::ShaderCompiler
{
struct Impl {
    Impl() {
        glslang::InitializeProcess();
    }
    ~Impl() {
        glslang::FinalizeProcess();
    }
};
Handle OCRA::ShaderCompiler::Create()
{
    return Handle(new Impl);
}
}

#pragma once

#include <OCRA/ShaderCompiler/Structs.hpp>

#include <glslang/Public/ShaderLang.h>

#include <vector>

namespace OCRA::ShaderCompiler {
struct Impl {
    Impl()
    {
        glslang::InitializeProcess();
    }
    ~Impl()
    {
        glslang::FinalizeProcess();
    }
    CompiledShader Compile(const ShaderInfo& a_Info);
    std::vector<uint32_t> CompileOpenGL(const ShaderInfo& a_Info);
    std::vector<uint32_t> CompileVulkan(const ShaderInfo& a_Info);
};
}

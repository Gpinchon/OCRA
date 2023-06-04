#include <OCRA/ShaderCompiler/Impl/Compiler.hpp>

#include <SPIRV/GlslangToSpv.h>
#include <glslang/Public/ResourceLimits.h>

#include <iostream>

std::vector<uint32_t> OCRA::ShaderCompiler::Impl::CompileVulkan(const ShaderInfo& a_Info)
{
    const auto eshLang = GetEshLang(a_Info.type);
    glslang::TShader shader(eshLang);
    const auto code = a_Info.source.data();
    shader.setStrings(&code, 1);
    shader.setEnvInput(glslang::EShSourceGlsl, eshLang, glslang::EShClientVulkan, 100);
    shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_0);
    shader.setEnvTarget(glslang::EshTargetSpv, glslang::EShTargetSpv_1_0);
    // shader.setEntryPoint(name.c_str());
    // shader.setSourceEntryPoint(name.c_str());
    if (!shader.parse(GetDefaultResources(), 100, false, EShMsgDefault))
        std::cerr << shader.getInfoLog() << std::endl;
    glslang::TProgram program;
    program.addShader(&shader);
    if (!program.link(EShMsgDefault))
        std::cerr << program.getInfoLog() << std::endl;
    spv::SpvBuildLogger logger;
    glslang::SpvOptions spvOptions;
    spvOptions.validate = true;
    std::vector<uint32_t> spvBinary;
    glslang::GlslangToSpv(*program.getIntermediate(eshLang), spvBinary, &logger, &spvOptions);
    return spvBinary;
}

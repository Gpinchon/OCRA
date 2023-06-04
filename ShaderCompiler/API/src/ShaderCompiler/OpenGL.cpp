#include <OCRA/ShaderCompiler/Impl/Compiler.hpp>

#include <SPIRV/GlslangToSpv.h>
#include <SPIRV/Logger.h>
#include <SPIRV/SpvTools.h>
#include <glslang\Public\ResourceLimits.h>
#include <spirv_glsl.hpp>

#include <cassert>
#include <iostream>

std::vector<uint32_t> OCRA::ShaderCompiler::Impl::CompileOpenGL(const ShaderInfo& a_Info)
{
    const auto eshLang = GetEshLang(a_Info.type);
    // Do some reflection
    std::string glslCode;
    {
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
        // SPIRV-Cross
        spirv_cross::CompilerGLSL glsl(spvBinary);
        spirv_cross::CompilerGLSL::Options options; // = glsl.get_common_options();
        options.emit_push_constant_as_uniform_buffer = true;
        glsl.set_common_options(options);
        const auto resources = glsl.get_shader_resources();
        for (const auto& res : resources.subpass_inputs) {
            throw std::runtime_error("Subpass inputs not supported");
        }
        for (const auto& res : resources.uniform_buffers) {
            auto set     = glsl.get_decoration(res.id, spv::DecorationDescriptorSet);
            auto binding = glsl.get_decoration(res.id, spv::DecorationBinding);
            assert(set == 0 && "Descriptor set unsupported, use set 0");
            assert(glsl.has_decoration(res.id, spv::DecorationBinding) && "Binding required");
            // glsl.set_decoration(res.id, spv::DecorationBinding, binding);
        }
        for (const auto& res : resources.push_constant_buffers) {
            glsl.set_decoration(res.id, spv::DecorationBinding, OCRA_GL_SHADER_PUSHCONSTANT_BINDING);
        }
        for (const auto& res : resources.separate_samplers) {
            auto set     = glsl.get_decoration(res.id, spv::DecorationDescriptorSet);
            auto binding = glsl.get_decoration(res.id, spv::DecorationBinding);
            assert(set == 0 && "Descriptor set unsupported, use set 0");
            assert(glsl.has_decoration(res.id, spv::DecorationBinding) && "Binding required");
            // glsl.set_decoration(res.id, spv::DecorationLocation, glsl.get_decoration(res.id, spv::DecorationBinding));
        }
        for (const auto& res : resources.sampled_images) {
            auto set     = glsl.get_decoration(res.id, spv::DecorationDescriptorSet);
            auto binding = glsl.get_decoration(res.id, spv::DecorationBinding);
            assert(set == 0 && "Descriptor set unsupported, use set 0");
            assert(glsl.has_decoration(res.id, spv::DecorationBinding) && "Binding required");
            // glsl.set_decoration(res.id, spv::DecorationLocation, glsl.get_decoration(res.id, spv::DecorationBinding));
        }
        // glsl.set_entry_point(name, GetExecutionModel(type));
        glslCode = glsl.compile();
    }
    // Generate final SPIRV blob
    std::vector<uint32_t> spvBinary;
    {
        glslang::TShader shader(eshLang);
        auto code = glslCode.data();
        shader.setStrings(&code, 1);
        shader.setEnvInput(glslang::EShSourceGlsl, eshLang, glslang::EShClientOpenGL, 100);
        shader.setEnvClient(glslang::EShClientOpenGL, glslang::EShTargetOpenGL_450);
        shader.setEnvTarget(glslang::EshTargetSpv, glslang::EShTargetSpv_1_0);
        shader.setEntryPoint(a_Info.entryPoint.c_str());
        shader.setSourceEntryPoint(a_Info.entryPoint.c_str());
        if (!shader.parse(GetDefaultResources(), 100, false, EShMsgDefault)) {
            const auto error = shader.getInfoLog();
            std::cerr << error << std::endl;
            throw std::runtime_error(error);
        }
        glslang::TProgram program;
        program.addShader(&shader);
        if (!program.link(EShMsgDefault)) {
            const auto error = program.getInfoLog();
            std::cerr << error << std::endl;
            throw std::runtime_error(error);
        }
        spv::SpvBuildLogger logger;
        glslang::SpvOptions spvOptions;
        spvOptions.validate = true;
        glslang::GlslangToSpv(*program.getIntermediate(eshLang), spvBinary, &logger, &spvOptions);
    }
    return spvBinary;
}

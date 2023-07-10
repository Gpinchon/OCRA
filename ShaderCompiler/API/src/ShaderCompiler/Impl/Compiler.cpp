#include <OCRA/ShaderCompiler/Impl/Compiler.hpp>
#include <OCRA/ShaderCompiler/Impl/Enums.hpp>

#include <spirv_glsl.hpp>

namespace OCRA::ShaderCompiler {
static inline ShaderResourceType BuildShaderResourceType(
    const spirv_cross::CompilerGLSL& a_GLSL,
    const spirv_cross::SPIRType& a_Type)
{
    ShaderResourceType type;
    type.rows      = a_Type.vecsize;
    type.columns   = a_Type.columns;
    type.byteWidth = a_Type.width / 8;
    type.baseType = ToOCRA(a_Type.basetype);
    type.arraySize = { a_Type.array.begin(), a_Type.array.end() };
    if (type.baseType == ShaderResourceBaseType::Struct) {
        auto& structure = type.structure.emplace();
        structure.byteSize = a_GLSL.get_declared_struct_size(a_Type);
        for (size_t index = 0; index < a_Type.member_types.size(); ++index) {
            auto& memberTypeID = a_Type.member_types[index];
            auto& memberType = a_GLSL.get_type(memberTypeID);
            structure.members.emplace_back(
                a_GLSL.get_member_name(a_Type.self, index),
                BuildShaderResourceType(a_GLSL, memberType)
            );
        }
    }
    return type;
}

static inline ShaderResource BuildShaderResource(const spirv_cross::CompilerGLSL& a_GLSL, const spirv_cross::Resource& a_Resource)
{
    ShaderResource resource;
    resource.name = a_GLSL.get_name(a_Resource.id);
    resource.location = a_GLSL.get_decoration(a_Resource.id, spv::DecorationLocation);
    resource.binding = a_GLSL.get_decoration(a_Resource.id, spv::DecorationBinding);
    resource.type = BuildShaderResourceType(a_GLSL, a_GLSL.get_type(a_Resource.type_id));
    return resource;
}
CompiledShader Impl::Compile(const ShaderInfo& a_Info)
{
    CompiledShader compiledShader = a_Info;
    if (a_Info.targetAPI == TargetAPI::OpenGL)
        compiledShader.SPIRVBinary = CompileOpenGL(a_Info);
    else if (a_Info.targetAPI == TargetAPI::Vulkan)
        compiledShader.SPIRVBinary = CompileVulkan(a_Info);
    else throw std::runtime_error("ShaderCompiler : Unknown target API");
    spirv_cross::CompilerGLSL glsl(compiledShader.SPIRVBinary);
    const auto resources = glsl.get_shader_resources();

    for (auto& res : resources.sampled_images) {
        compiledShader.sampledImages.push_back(compiledShader.resources.size());
        compiledShader.resources.push_back(BuildShaderResource(glsl, res));
    }
    for (auto& res : resources.separate_images) {
        compiledShader.separateImages.push_back(compiledShader.resources.size());
        compiledShader.resources.push_back(BuildShaderResource(glsl, res));
    }
    for (auto& res : resources.stage_inputs) {
        compiledShader.stageInputs.push_back(compiledShader.resources.size());
        compiledShader.resources.push_back(BuildShaderResource(glsl, res));
    }
    for (auto& res : resources.stage_outputs) {
        compiledShader.stageOutputs.push_back(compiledShader.resources.size());
        compiledShader.resources.push_back(BuildShaderResource(glsl, res));
    }
    for (auto& res : resources.storage_buffers) {
        compiledShader.storageBuffers.push_back(compiledShader.resources.size());
        compiledShader.resources.push_back(BuildShaderResource(glsl, res));
    }
    for (auto& res : resources.storage_images) {
        compiledShader.storageImages.push_back(compiledShader.resources.size());
        compiledShader.resources.push_back(BuildShaderResource(glsl, res));
    }
    for (auto& res : resources.uniform_buffers) {
        if (a_Info.targetAPI == TargetAPI::OpenGL && glsl.get_decoration(res.id, spv::DecorationBinding) == OCRA_GL_SHADER_PUSHCONSTANT_BINDING) {
            // this is not a uniform buffer, this is the reserved push constant binding
            compiledShader.pushConstants.push_back(compiledShader.resources.size());
            compiledShader.resources.push_back(BuildShaderResource(glsl, res));
        }
        else {
            compiledShader.uniformBuffers.push_back(compiledShader.resources.size());
            compiledShader.resources.push_back(BuildShaderResource(glsl, res));
        }
    }
    if (a_Info.targetAPI == TargetAPI::Vulkan) {
        for (auto& res : resources.push_constant_buffers) {
            compiledShader.pushConstants.push_back(compiledShader.resources.size());
            compiledShader.resources.push_back(BuildShaderResource(glsl, res));
        }
    }
    return compiledShader;
}
}

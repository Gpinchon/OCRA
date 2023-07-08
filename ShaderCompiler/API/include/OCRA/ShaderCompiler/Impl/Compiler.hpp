#pragma once

#include <OCRA/ShaderCompiler/Structs.hpp>

#include <stdexcept>
#include <vector>

#include <glslang/Public/ShaderLang.h>
#include <spirv.hpp>
#include <spirv_glsl.hpp>

namespace OCRA::ShaderCompiler {
static inline auto ToOCRA(const spirv_cross::SPIRType::BaseType& a_Type)
{
    switch (a_Type) {
    case spirv_cross::SPIRType::BaseType::Void:
        return ShaderResourceBaseType::Void;
    case spirv_cross::SPIRType::BaseType::Boolean:
        return ShaderResourceBaseType::Boolean;
    case spirv_cross::SPIRType::BaseType::SByte:
        return ShaderResourceBaseType::SByte;
    case spirv_cross::SPIRType::BaseType::UByte:
        return ShaderResourceBaseType::UByte;
    case spirv_cross::SPIRType::BaseType::Short:
        return ShaderResourceBaseType::Short;
    case spirv_cross::SPIRType::BaseType::UShort:
        return ShaderResourceBaseType::UShort;
    case spirv_cross::SPIRType::BaseType::Int:
        return ShaderResourceBaseType::Int;
    case spirv_cross::SPIRType::BaseType::UInt:
        return ShaderResourceBaseType::UInt;
    case spirv_cross::SPIRType::BaseType::Int64:
        return ShaderResourceBaseType::Int64;
    case spirv_cross::SPIRType::BaseType::UInt64:
        return ShaderResourceBaseType::UInt64;
    case spirv_cross::SPIRType::BaseType::AtomicCounter:
        return ShaderResourceBaseType::AtomicCounter;
    case spirv_cross::SPIRType::BaseType::Half:
        return ShaderResourceBaseType::Half;
    case spirv_cross::SPIRType::BaseType::Float:
        return ShaderResourceBaseType::Float;
    case spirv_cross::SPIRType::BaseType::Double:
        return ShaderResourceBaseType::Double;
    case spirv_cross::SPIRType::BaseType::Struct:
        return ShaderResourceBaseType::Struct;
    case spirv_cross::SPIRType::BaseType::Image:
        return ShaderResourceBaseType::Image;
    case spirv_cross::SPIRType::BaseType::SampledImage:
        return ShaderResourceBaseType::SampledImage;
    case spirv_cross::SPIRType::BaseType::Sampler:
        return ShaderResourceBaseType::Sampler;
    case spirv_cross::SPIRType::BaseType::AccelerationStructure:
        return ShaderResourceBaseType::AccelerationStructure;
    case spirv_cross::SPIRType::BaseType::RayQuery:
        return ShaderResourceBaseType::RayQuery;
    }
    return ShaderResourceBaseType::Unknown;
}

static inline ShaderResourceType BuildShaderResourceType(
    const spirv_cross::CompilerGLSL& a_GLSL,
    const spirv_cross::Resource& a_Resource,
    const spirv_cross::SPIRType& a_Type)
{
    ShaderResourceType type;
    type.rows     = a_Type.vecsize;
    type.columns  = a_Type.columns;
    type.width    = a_Type.width;
    type.baseType = ToOCRA(a_Type.basetype);

    if (type.baseType == ShaderResourceBaseType::Struct) {
        auto& structure    = type.structure.emplace();
        structure.byteSize = a_GLSL.get_declared_struct_size(a_Type);
        for (size_t index = 0; index < a_Type.member_types.size(); ++index) {
            auto& memberTypeID = a_Type.member_types[index];
            structure.members.push_back({ a_GLSL.get_member_name(a_Resource.base_type_id, index),
                BuildShaderResourceType(a_GLSL, a_Resource, a_GLSL.get_type(memberTypeID)) });
        }
    }
    return type;
}

static inline ShaderResource BuildShaderResource(const spirv_cross::CompilerGLSL& a_GLSL, const spirv_cross::Resource& a_Resource)
{
    ShaderResource resource;
    resource.name     = a_GLSL.get_name(a_Resource.id);
    resource.location = a_GLSL.get_decoration(a_Resource.id, spv::DecorationLocation);
    resource.binding  = a_GLSL.get_decoration(a_Resource.id, spv::DecorationBinding);
    resource.type     = BuildShaderResourceType(a_GLSL, a_Resource, a_GLSL.get_type(a_Resource.type_id));
    return resource;
}
struct Impl {
    Impl()
    {
        glslang::InitializeProcess();
    }
    ~Impl()
    {
        glslang::FinalizeProcess();
    }
    CompiledShader Compile(const ShaderInfo& a_Info)
    {
        CompiledShader compiledShader = a_Info;
        if (a_Info.targetAPI == TargetAPI::OpenGL)
            compiledShader.SPIRVBinary = CompileOpenGL(a_Info);
        else if (a_Info.targetAPI == TargetAPI::Vulkan) {
            compiledShader.SPIRVBinary = CompileVulkan(a_Info);
        } else
            throw std::runtime_error("ShaderCompiler : Unknown target API");
        spirv_cross::CompilerGLSL glsl(compiledShader.SPIRVBinary);
        const auto resources = glsl.get_shader_resources();
        for (auto& res : resources.sampled_images) {
            compiledShader.resources.sampledImages.push_back(compiledShader.resources.resources.size());
            compiledShader.resources.resources.push_back(BuildShaderResource(glsl, res));
        }
        for (auto& res : resources.uniform_buffers) {
            if (a_Info.targetAPI == TargetAPI::OpenGL && glsl.get_decoration(res.id, spv::DecorationBinding) == OCRA_GL_SHADER_PUSHCONSTANT_BINDING) {
                // this is not a uniform buffer
                compiledShader.resources.pushConstants.push_back(compiledShader.resources.resources.size());
                compiledShader.resources.resources.push_back(BuildShaderResource(glsl, res));
            } else {
                compiledShader.resources.uniformBuffers.push_back(compiledShader.resources.resources.size());
                compiledShader.resources.resources.push_back(BuildShaderResource(glsl, res));
            }
        }
        for (auto& res : resources.stage_inputs) {
            compiledShader.resources.stageInputs.push_back(compiledShader.resources.resources.size());
            compiledShader.resources.resources.push_back(BuildShaderResource(glsl, res));
        }
        for (auto& res : resources.stage_outputs) {
            compiledShader.resources.stageOutputs.push_back(compiledShader.resources.resources.size());
            compiledShader.resources.resources.push_back(BuildShaderResource(glsl, res));
        }
        if (a_Info.targetAPI == TargetAPI::Vulkan) {
            for (auto& res : resources.push_constant_buffers) {
                compiledShader.resources.pushConstants.push_back(compiledShader.resources.resources.size());
                compiledShader.resources.resources.push_back(BuildShaderResource(glsl, res));
            }
        }
        return compiledShader;
    }
    std::vector<uint32_t> CompileOpenGL(const ShaderInfo& a_Info);
    std::vector<uint32_t> CompileVulkan(const ShaderInfo& a_Info);
    static auto inline GetExecutionModel(const ShaderType& a_Type)
    {
        switch (a_Type) {
        case ShaderType::Vertex:
            return spv::ExecutionModel::ExecutionModelVertex;
        case ShaderType::Geometry:
            return spv::ExecutionModel::ExecutionModelGeometry;
        case ShaderType::Fragment:
            return spv::ExecutionModel::ExecutionModelFragment;
        case ShaderType::Compute:
            return spv::ExecutionModel::ExecutionModelGLCompute;
        case ShaderType::TessControl:
            return spv::ExecutionModel::ExecutionModelTessellationControl;
        case ShaderType::TessEval:
            return spv::ExecutionModel::ExecutionModelTessellationEvaluation;
        }
        throw std::runtime_error("Unknown shader type !");
    }
    static auto inline GetEshLang(const ShaderType& a_Type)
    {
        switch (a_Type) {
        case ShaderType::Vertex:
            return EShLangVertex;
        case ShaderType::Geometry:
            return EShLangGeometry;
        case ShaderType::Fragment:
            return EShLangFragment;
        case ShaderType::Compute:
            return EShLangCompute;
        case ShaderType::TessControl:
            return EShLangTessControl;
        case ShaderType::TessEval:
            return EShLangTessEvaluation;
        }
        throw std::runtime_error("Unknown shader type !");
    }
};
}

#pragma once

#include <OCRA/ShaderCompiler/Enums.hpp>

#include <glslang/Public/ShaderLang.h>
#include <spirv.hpp>
#include <spirv_cross.hpp>

namespace OCRA::ShaderCompiler {
constexpr auto ToOCRA(const spirv_cross::SPIRType::BaseType& a_Type)
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

constexpr auto ToSPV(const ShaderType& a_Type)
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

constexpr auto ToGLSLang(const ShaderType& a_Type)
{
    switch (a_Type) {
    case ShaderType::Vertex:
        return EShLanguage::EShLangVertex;
    case ShaderType::Geometry:
        return EShLanguage::EShLangGeometry;
    case ShaderType::Fragment:
        return EShLanguage::EShLangFragment;
    case ShaderType::Compute:
        return EShLanguage::EShLangCompute;
    case ShaderType::TessControl:
        return EShLanguage::EShLangTessControl;
    case ShaderType::TessEval:
        return EShLanguage::EShLangTessEvaluation;
    }
    throw std::runtime_error("Unknown shader type !");
}
}
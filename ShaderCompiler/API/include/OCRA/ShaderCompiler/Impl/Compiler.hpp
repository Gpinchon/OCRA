#pragma once

#include <OCRA/ShaderCompiler/Structs.hpp>

#include <vector>
#include <stdexcept>

#include <glslang/Public/ShaderLang.h>
#include <spirv.hpp>

namespace OCRA::ShaderCompiler
{
struct Impl {
	Impl() {
		glslang::InitializeProcess();
	}
	~Impl() {
		glslang::FinalizeProcess();
	}
	std::vector<uint32_t> Compile(const ShaderInfo& a_Info) {
		if (a_Info.targetAPI == TargetAPI::OpenGL)
			return CompileOpenGL(a_Info);
		else if (a_Info.targetAPI == TargetAPI::Vulkan)
			return CompileVulkan(a_Info);
		throw std::runtime_error("ShaderCompiler : Unknown target API");
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
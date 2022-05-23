#include <ShaderCompiler/Shader.hpp>

#include <glslang\Public\ShaderLang.h>
#include <SPIRV\GlslangToSpv.h>
#include <StandAlone\ResourceLimits.h>
#include <spirv_cross.hpp>
#include <spirv_glsl.hpp>

#include <stdexcept>
#include <iostream>
#include <sstream>

namespace OCRA::ShaderCompiler::Shader
{
const auto inline GetExecutionModel(const Shader::Type& a_Type)
{
	switch (a_Type) {
	case Shader::Type::Vertex:
		return spv::ExecutionModel::ExecutionModelVertex;
	case Shader::Type::Geometry:
		return spv::ExecutionModel::ExecutionModelGeometry;
	case Shader::Type::Fragment:
		return spv::ExecutionModel::ExecutionModelFragment;
	case Shader::Type::Compute:
		return spv::ExecutionModel::ExecutionModelGLCompute;
	case Shader::Type::TessControl:
		return spv::ExecutionModel::ExecutionModelTessellationControl;
	case Shader::Type::TessEval:
		return spv::ExecutionModel::ExecutionModelTessellationEvaluation;
	}
	throw std::runtime_error("Unknown shader type !");
}

const auto inline GetEshLang(const Shader::Type& a_Type)
{
	switch (a_Type) {
	case Shader::Type::Vertex:
		return EShLangVertex;
	case Shader::Type::Geometry:
		return EShLangGeometry;
	case Shader::Type::Fragment:
		return EShLangFragment;
	case Shader::Type::Compute:
		return EShLangCompute;
	case Shader::Type::TessControl:
		return EShLangTessControl;
	case Shader::Type::TessEval:
		return EShLangTessEvaluation;
	}
	throw std::runtime_error("Unknown shader type !");
}

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
	const auto eshLang = GetEshLang(info.type);
	//Do some reflection
	std::string glslCode;
	{
		glslang::TShader shader(eshLang);
		const auto code = info.source.data();
		shader.setStrings(&code, 1);
		shader.setEnvInput(glslang::EShSourceGlsl, eshLang, glslang::EShClientVulkan, 100);
		shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_0);
		shader.setEnvTarget(glslang::EshTargetSpv, glslang::EShTargetSpv_1_0);
		//shader.setEntryPoint(name.c_str());
		//shader.setSourceEntryPoint(name.c_str());
		if (!shader.parse(&glslang::DefaultTBuiltInResource, 100, false, EShMsgDefault))
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
		//SPIRV-Cross
		spirv_cross::CompilerGLSL glsl(spvBinary);
		//glsl.set_entry_point(name, GetExecutionModel(type));
		//TODO : do some reflection here
		glslCode = glsl.compile();
	}
	//Generate final SPIRV blob
	std::vector<uint32_t> spvBinary;
	{
		glslang::TShader shader(eshLang);
		auto code = glslCode.data();
		shader.setStrings(&code, 1);
		shader.setEnvInput(glslang::EShSourceGlsl, eshLang, glslang::EShClientOpenGL, 100);
		shader.setEnvClient(glslang::EShClientOpenGL, glslang::EShTargetOpenGL_450);
		shader.setEnvTarget(glslang::EshTargetSpv, glslang::EShTargetSpv_1_0);
		shader.setEntryPoint(info.entryPoint.c_str());
		shader.setSourceEntryPoint(info.entryPoint.c_str());
		if (!shader.parse(&glslang::DefaultTBuiltInResource, 100, false, EShMsgDefault))
			std::cerr << shader.getInfoLog() << std::endl;
		glslang::TProgram program;
		program.addShader(&shader);
		if (!program.link(EShMsgDefault))
			std::cerr << program.getInfoLog() << std::endl;
		spv::SpvBuildLogger logger;
		glslang::SpvOptions spvOptions;
		spvOptions.validate = true;
		glslang::GlslangToSpv(*program.getIntermediate(eshLang), spvBinary, &logger, &spvOptions);
	}
	return spvBinary;
}
Handle Create(const ShaderCompiler::Handle& a_Compiler, const Info& a_Info) {
	return Handle(new Impl(a_Compiler, a_Info));
}
std::vector<uint32_t> Compile(const Handle& a_Shader) {
	return a_Shader->Compile();
}
}
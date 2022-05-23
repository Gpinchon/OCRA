#include <ShaderCompiler/Compiler.hpp>
#include <ShaderCompiler/Shader.hpp>

using namespace OCRA;

int main()
{
	int ret = 0;
	auto compiler = ShaderCompiler::Create();
	{
		std::string shaderCode{
			"#version 430\n"
			"void emptyShader() {}\n"
			"void main() { emptyShader(); }\n"
		};
		ShaderCompiler::Shader::Info shaderInfo;
		shaderInfo.type = ShaderCompiler::Shader::Type::Fragment;
		shaderInfo.entryPoint = "emptyShader";
		shaderInfo.source = shaderCode;
		const auto shader = ShaderCompiler::Shader::Create(compiler, shaderInfo);
		ret |= ShaderCompiler::Shader::Compile(shader).empty() ? -1 : 0;
	}
	{
		std::string shaderCode{
			"#version 430\n"
			"layout(set = 0, binding = 0, input_attachment_index = 0) uniform subpassInput uLastColor;\n"
			"layout(location = 0) out vec4 color;\n"
			"void subpassShader() {\n"
			"	color = subpassLoad(uLastColor);\n"
			"}\n"
			"void main() { subpassShader(); }\n"
		};
		ShaderCompiler::Shader::Info shaderInfo;
		shaderInfo.type = ShaderCompiler::Shader::Type::Fragment;
		shaderInfo.entryPoint = "subpassShader";
		shaderInfo.source = shaderCode;
		const auto shader = ShaderCompiler::Shader::Create(compiler, shaderInfo);
		ret |= ShaderCompiler::Shader::Compile(shader).empty() ? -1 : 0;
	}
    return ret;
}
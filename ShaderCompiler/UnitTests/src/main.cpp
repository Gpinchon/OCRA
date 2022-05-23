#include <ShaderCompiler/Shader.hpp>

using namespace OCRA;

int main()
{
	{
		std::string shaderCode{
			"#version 430\n"
			"void emptyShader() {}\n"
			"void main(){ emptyShader(); }"
		};
		ShaderCompiler::FragmentShader shader("emptyShader", shaderCode);
		shader.Compile(true);
	}
	{
		std::string shaderCode{
			"layout(set = 0, binding = 0, input_attachment_index = 0) uniform subpassInput uLastColor;\n"
			"layout(location = 0) out vec4 color;\n"
			"void subpassShader() {\n"
			"	color = subpassLoad(uLastColor);"
			"}\n"
		};
		ShaderCompiler::FragmentShader shader("subpassShader", shaderCode);
		shader.Compile(true);
	}
    return 0;
}
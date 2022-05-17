#include <Shader.hpp>

using namespace OCRA;

int main()
{
    std::string shaderCode{
        "#version 430\n"
        "void main() {}\n"
    };
    ShaderCompiler::FragmentShader shader("testShader", shaderCode);
    shader.Compile(true);
    return 0;
}
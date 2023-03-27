#include <OCRA/ShaderCompiler/Compiler.hpp>
#include <OCRA/ShaderCompiler/Shader.hpp>

#include <iostream>

using namespace OCRA;

int main()
{
    int ret = 0;
    auto compiler = ShaderCompiler::Create();
    {
        ShaderCompiler::Shader::Info shaderInfo;
        shaderInfo.type = ShaderCompiler::Shader::Type::Fragment;
        shaderInfo.entryPoint = "emptyShader";
        shaderInfo.source = {
            "#version 430                   \n"
            "void emptyShader() {}          \n"
            "void main() { emptyShader(); } \n"
        };
        const auto shader = ShaderCompiler::Shader::Create(compiler, shaderInfo);
        ret |= ShaderCompiler::Shader::Compile(shader).empty() ? -1 : 0;
    }
    {
        ShaderCompiler::Shader::Info shaderInfo;
        shaderInfo.type = ShaderCompiler::Shader::Type::Vertex;
        shaderInfo.entryPoint = "vertexIndex";
        shaderInfo.source = {
            "#version 430                                           \n"
            "const vec2 positions[3] = vec2[](                      \n"
            "   vec2( 0.0, -0.5),                                   \n"
            "   vec2( 0.5, -0.5),                                   \n"
            "   vec2(-0.5,  0.5)                                    \n"
            ");                                                     \n"
            "void vertexIndex() {                                   \n"
            "   gl_Position = vec4(positions[gl_VertexIndex], 0, 1);\n"
            "}                                                      \n"
            "void main() {                                          \n"
            "   vertexIndex();                                      \n"
            "}                                                      \n"
        };
        const auto shader = ShaderCompiler::Shader::Create(compiler, shaderInfo);
        ret |= ShaderCompiler::Shader::Compile(shader).empty() ? -1 : 0;
    }
    {
        ShaderCompiler::Shader::Info shaderInfo;
        shaderInfo.type = ShaderCompiler::Shader::Type::Vertex;
        shaderInfo.entryPoint = "pushConstant";
        shaderInfo.source = {
            "#version 430                                                   \n"
            "layout(location = 0) in vec3 vPosition;                        \n"
            "layout(push_constant) uniform constants {                      \n"
            "   mat4 viewMatrix;                                            \n"
            "} PushConstants;                                               \n"
            "void pushConstant() {                                          \n"
            "   gl_Position = PushConstants.viewMatrix * vec4(vPosition, 1);\n"
            "}                                                              \n"
            "void main() {                                                  \n"
            "   pushConstant();                                             \n"
            "}                                                              \n"
        };
        const auto shader = ShaderCompiler::Shader::Create(compiler, shaderInfo);
        ret |= ShaderCompiler::Shader::Compile(shader).empty() ? -1 : 0;
    }
    {
        ShaderCompiler::Shader::Info shaderInfo;
        shaderInfo.type = ShaderCompiler::Shader::Type::Vertex;
        shaderInfo.entryPoint = "instanceIndex";
        shaderInfo.source = {
            "#version 430                                                   \n"
            "const vec2 positions0[3] = vec2[](                             \n"
            "   vec2( 0.0, -0.5),                                           \n"
            "   vec2( 0.5, -0.5),                                           \n"
            "   vec2(-0.5,  0.5)                                            \n"
            ");                                                             \n"
            "const vec2 positions1[3] = vec2[](                             \n"
            "   vec2( 0.00, -0.25),                                         \n"
            "   vec2( 0.25, -0.25),                                         \n"
            "   vec2(-0.25,  0.25)                                          \n"
            ");                                                             \n"
            "void instanceIndex() {                                         \n"
            "   if (gl_InstanceIndex == 0)                                  \n"
            "       gl_Position = vec4(positions0[gl_InstanceIndex], 0, 1); \n"
            "   else                                                        \n"
            "       gl_Position = vec4(positions1[gl_InstanceIndex], 0, 1); \n"
            "}                                                              \n"
            "void main() {                                                  \n"
            "   instanceIndex();                                            \n"
            "}                                                              \n"
        };
        const auto shader = ShaderCompiler::Shader::Create(compiler, shaderInfo);
        ret |= ShaderCompiler::Shader::Compile(shader).empty() ? -1 : 0;
    }
    try
    {
        ShaderCompiler::Shader::Info shaderInfo;
        shaderInfo.type = ShaderCompiler::Shader::Type::Fragment;
        shaderInfo.entryPoint = "subpassShader";
        shaderInfo.source = {
            "#version 430                                                                      \n"
            "layout(binding = 0, input_attachment_index = 0) uniform subpassInput uLastColor;  \n"
            "layout(location = 0) out vec4 color;                                              \n"
            "void subpassShader() {                                                            \n"
            "   color = subpassLoad(uLastColor);                                               \n"
            "}                                                                                 \n"
            "void main() { subpassShader(); }                                                  \n"
        };
        const auto shader = ShaderCompiler::Shader::Create(compiler, shaderInfo);
        ret |= ShaderCompiler::Shader::Compile(shader).empty() ? -1 : 0;
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    {
        ShaderCompiler::Shader::Info shaderInfo;
        shaderInfo.type = ShaderCompiler::Shader::Type::Fragment;
        shaderInfo.entryPoint = "multipleSet";
        shaderInfo.source = {
            "#version 430                                                                      \n"
            "layout(binding = 0, input_attachment_index = 0) uniform subpassInput uLastColor;  \n"
            "layout(binding = 1, input_attachment_index = 1) uniform subpassInput uLastDepth;  \n"
            "layout(location = 0) out vec4 color;                                              \n"
            "void multipleSet() {                                                              \n"
            "   color = subpassLoad(uLastColor);                                               \n"
            "   gl_FragDepth = subpassLoad(uLastDepth).r;                                      \n"
            "}                                                                                 \n"
            "void main() { multipleSet(); }                                                    \n"
        };
        const auto shader = ShaderCompiler::Shader::Create(compiler, shaderInfo);
        ret |= ShaderCompiler::Shader::Compile(shader).empty() ? -1 : 0;
    }
    {
        ShaderCompiler::Shader::Info shaderInfo;
        shaderInfo.type = ShaderCompiler::Shader::Type::Vertex;
        shaderInfo.entryPoint = "uniformLocation";
        shaderInfo.source = {
            "#version 430                                               \n"
            "layout(location = 0) in vec3 vPosition;                    \n"
            "layout(binding = 0) uniform sampler2D uTexture;            \n"
            "layout(binding = 0) uniform camera {                       \n"
            "   mat4 viewMatrix;                                        \n"
            "} uCamera;                                                 \n"
            "void uniformLocation() {                                   \n"
            "   gl_Position = uCamera.viewMatrix * vec4(vPosition, 1);  \n"
            "}                                                          \n"
            "void main() {                                              \n"
            "   uniformLocation();                                      \n"
            "}                                                          \n"
        };
        const auto shader = ShaderCompiler::Shader::Create(compiler, shaderInfo);
        ret |= ShaderCompiler::Shader::Compile(shader).empty() ? -1 : 0;
    }
    return ret;
}
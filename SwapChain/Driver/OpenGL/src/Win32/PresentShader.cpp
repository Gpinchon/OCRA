#include <GL/Win32/PresentShader.hpp>

#include <GL/glew.h>

#include <string.h>
#include <stdexcept>

namespace OCRA::SwapChain::Win32
{
static inline auto CheckShaderCompilation(GLuint a_Shader)
{
    GLint result;
    glGetShaderiv(a_Shader, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE) {
        GLsizei length{ 0 };
        glGetShaderiv(a_Shader, GL_INFO_LOG_LENGTH, &length);
        if (length > 1) {
            auto infoLog = new char[length];
            glGetShaderInfoLog(a_Shader, length, nullptr, infoLog);
            std::string logString(infoLog, infoLog + length);
            delete infoLog;
            throw std::runtime_error(logString);
        }
        else throw std::runtime_error("Unknown Error");
        return false;
    }
    return true;
}

PresentShader::PresentShader()
{
    const auto vertCode =
        "#version 430                                       \n"
        "out vec2 UV;                                       \n"
        "void main() {                                      \n"
        "   float x = -1.0 + float((gl_VertexID & 1) << 2); \n"
        "   float y = -1.0 + float((gl_VertexID & 2) << 1); \n"
        "   UV.x = (x + 1.0) * 0.5;                         \n"
        "   UV.y = 1 - (y + 1.0) * 0.5;                     \n"
        "   gl_Position = vec4(x, y, 0, 1);                 \n"
        "}                                                  \n"
        ;
    const int vertLen = strlen(vertCode);
    const auto vertHandle = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertHandle, 1, &vertCode, &vertLen);
    glCompileShader(vertHandle);
    CheckShaderCompilation(vertHandle);

    const auto fragCode =
        "#version 430                                       \n"
        "layout(location = 0) out vec4 out_Color;           \n"
        "layout(binding = 0) uniform sampler2D in_Color;    \n"
        "in vec2 UV;                                        \n"
        "void main() {                                      \n"
        "   out_Color = texture(in_Color, UV);              \n"
        "}                                                  \n"
        ;
    const int fragLen = strlen(fragCode);
    const auto fragHandle = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragHandle, 1, &fragCode, &fragLen);
    glCompileShader(fragHandle);
    CheckShaderCompilation(fragHandle);

    handle = glCreateProgram();
    glAttachShader(handle, vertHandle);
    glAttachShader(handle, fragHandle);
    glLinkProgram(handle);
    glDetachShader(handle, vertHandle);
    glDetachShader(handle, fragHandle);
    glDeleteShader(vertHandle);
    glDeleteShader(fragHandle);
}
PresentShader::~PresentShader()
{
    glDeleteProgram(handle);
}
void PresentShader::Bind() const
{
    glUseProgram(handle);
}
void PresentShader::Unbind() const
{
    glUseProgram(0);
}
}
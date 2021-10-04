/*
* @Author: gpinchon
* @Date:   2021-10-04 20:31:19
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-04 20:35:26
*/
#include <Handle.hpp>
#include <Shader/Stage.hpp>

#include <map>

#include <GL/Shader/Stage.hpp>
#include <GL/glew.h>

namespace OCRA::Shader::Stage {
static inline auto CheckCompilation(GLuint program)
{
    GLint result;
    glGetProgramiv(program, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE) {
        GLsizei length { 0 };
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        if (length > 1) {
            std::vector<char> infoLog(length, 0);
            glGetProgramInfoLog(program, length, nullptr, infoLog.data());
            std::string logString(infoLog.begin(), infoLog.end());
            throw std::runtime_error(logString);
        } else
            throw std::runtime_error("Unknown Error");
    }
}
struct Impl {
    Impl(const Device::Handle& a_Device, const Info& a_Info)
        : info(a_Info)
    {
        const auto codeBuffer = info.code.data();
        handle = glCreateShaderProgramv(GetGLStage(info.stage), 1, &codeBuffer);
        CheckCompilation(handle);
    }
    ~Impl()
    {
        glDeleteProgram(handle);
    }
    const Info info;
    GLuint handle;
};
static Handle s_CurrentHandle = 0;
static std::map<Handle, Impl> s_Stages;
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
    ++s_CurrentHandle;
    s_Stages.emplace(s_CurrentHandle, Impl(a_Device, a_Info));
    return s_CurrentHandle;
}
void Destroy(const Device::Handle& a_Device, const Handle& a_Handle)
{
    s_Stages.erase(a_Handle);
}
const Info& GetInfo(const Device::Handle& a_Device, const Handle& a_Handle)
{
    return s_Stages.at(a_Handle).info;
}
unsigned GetGLHandle(const Device::Handle& a_Device, const Handle& a_Handle)
{
    return s_Stages.at(a_Handle).handle;
}
}
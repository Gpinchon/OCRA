/*
* @Author: gpinchon
* @Date:   2021-10-04 20:31:19
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-04 20:35:26
*/
#include <Handle.hpp>
#include <Shader/Stage.hpp>

#include <GL/Shader/Stage.hpp>
#include <GL/Device.hpp>
#include <GL/WeakHandle.hpp>
#include <GL/glew.h>

#include <cassert>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

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
        } else throw std::runtime_error("Unknown Error");
        return false;
    }
    return true;
}
struct Impl {
    Impl(const Device::Handle& a_Device, const Info& a_Info)
        : device(a_Device)
        , info(a_Info)
        , program(glCreateProgram())
    {
        std::vector<uint32_t> constantIndex;
        std::vector<uint32_t> constantValue;
        constantIndex.reserve(info.specializationInfo.mapEntries.size());
        constantValue.reserve(info.specializationInfo.mapEntries.size());
        for (const auto& entry : info.specializationInfo.mapEntries) {
            uint32_t value{ 0 };
            assert(entry.size <= sizeof(value));
            auto data = &info.specializationInfo.data.at(entry.offset);
            std::memcpy(&value, data, entry.size);
            constantValue.push_back(value);
            constantIndex.push_back(entry.constantID);
        }
        a_Device->PushCommand([this, moduleInfo = Module::GetInfo(info.module), constantIndex, constantValue] {
            const auto shader = glCreateShader(GetGLStage(info.stage));
            glProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE);
            //ARB_gl_spirv
            glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, moduleInfo.code.data(), moduleInfo.code.size());
            glSpecializeShader(shader, info.name.c_str(), constantIndex.size(), constantIndex.data(), constantValue.data());
            glCompileShader(shader);
            if (CheckCompilation(shader)) //compilation is successful
            {
                glAttachShader(program, shader);
                glLinkProgram(program);
                glDetachShader(program, shader);
            }
            glDeleteShader(shader);
        }, true);
    }
    ~Impl()
    {
        device.lock()->PushCommand([program = program] {
            glDeleteProgram(program);
        }, false);
    }
    const Device::WeakHandle device;
    const Info info;
    const GLuint program;
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
    return Handle(new Impl(a_Device, a_Info));
}
const Info& GetInfo(const Handle& a_Handle)
{
    return a_Handle->info;
}
unsigned GetGLHandle(const Handle& a_Handle)
{
    return a_Handle->program;
}
}
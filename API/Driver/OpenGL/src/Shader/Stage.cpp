/*
* @Author: gpinchon
* @Date:   2021-10-04 20:31:19
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-04 20:35:26
*/
#include <OCRA/Handle.hpp>
#include <OCRA/Shader/Stage.hpp>

#include <GL/Common/Assert.hpp>
#include <GL/Shader/Stage.hpp>
#include <GL/Device.hpp>

#include <GL/glew.h>

#include <stdexcept>

namespace OCRA::Shader::Stage {
static inline auto GetGLStageBits(const StageFlags& a_Stage)
{
    uint32_t shaderStages = 0;
    if ((a_Stage & StageFlagBits::Vertex) != 0)
        shaderStages |= GL_VERTEX_SHADER_BIT;
    if ((a_Stage & StageFlagBits::Geometry) != 0)
        shaderStages |= GL_GEOMETRY_SHADER_BIT;
    if ((a_Stage & StageFlagBits::Fragment) != 0)
        shaderStages |= GL_FRAGMENT_SHADER_BIT;
    if ((a_Stage & StageFlagBits::Compute) != 0)
        shaderStages |= GL_COMPUTE_SHADER_BIT;
    if ((a_Stage & StageFlagBits::TessControl) != 0)
        shaderStages |= GL_TESS_CONTROL_SHADER_BIT;
    if ((a_Stage & StageFlagBits::TessEval) != 0)
        shaderStages |= GL_TESS_EVALUATION_SHADER_BIT;
    return shaderStages;
}

static inline auto GetGLStage(const StageFlags& a_Stage)
{
    if (a_Stage == StageFlagBits::Vertex)
        return GL_VERTEX_SHADER;
    if (a_Stage == StageFlagBits::Geometry)
        return GL_GEOMETRY_SHADER;
    if (a_Stage == StageFlagBits::Fragment)
        return GL_FRAGMENT_SHADER;
    if (a_Stage == StageFlagBits::Compute)
        return GL_COMPUTE_SHADER;
    if (a_Stage == StageFlagBits::TessControl)
        return GL_TESS_CONTROL_SHADER;
    if (a_Stage == StageFlagBits::TessEval)
        return GL_TESS_EVALUATION_SHADER;
    throw std::runtime_error("Unknown Shader Stage");
}

static inline auto CheckShaderCompilation(GLuint a_Shader)
{
    GLint result;
    glGetShaderiv(a_Shader, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE) {
        GLsizei length { 0 };
        glGetShaderiv(a_Shader, GL_INFO_LOG_LENGTH, &length);
        if (length > 1) {
            std::vector<char> infoLog(length, 0);
            glGetShaderInfoLog(a_Shader, length, nullptr, infoLog.data());
            std::string logString(infoLog.begin(), infoLog.end());
            throw std::runtime_error(logString);
        } else throw std::runtime_error("Unknown Error");
        return false;
    }
    return true;
}

static inline auto CreateShaderProgram(const Device::Handle& a_Device)
{
    uint32_t handle = 0;
    a_Device->PushCommand([&handle] {
        handle = glCreateProgram();
    }, true);
    return handle;
}

Impl::Impl(const Device::Handle& a_Device, const Info& a_Info)
    : device(a_Device)
    , info(a_Info)
    , handle(CreateShaderProgram(a_Device))
    , stage(GetGLStage(info.stage))
    , stageBits(GetGLStageBits(info.stage))
{
    std::vector<uint32_t> constantIndex;
    std::vector<uint32_t> constantValue;
    constantIndex.reserve(info.specializationInfo.mapEntries.size());
    constantValue.reserve(info.specializationInfo.mapEntries.size());
    for (const auto& entry : info.specializationInfo.mapEntries) {
        uint32_t value{ 0 };
        OCRA_ASSERT(entry.size <= sizeof(value));
        auto data = &info.specializationInfo.data.at(entry.offset);
        std::memcpy(&value, data, entry.size);
        constantValue.push_back(value);
        constantIndex.push_back(entry.constantID);
    }
    a_Device->PushCommand([this, moduleInfo = Module::GetInfo(info.module), constantIndex, constantValue] {
        const auto shader = glCreateShader(stage);
        glProgramParameteri(handle, GL_PROGRAM_SEPARABLE, GL_TRUE);
        OCRA_ASSERT(GLEW_ARB_gl_spirv);
        glShaderBinary(1, &shader, GL_SHADER_BINARY_FORMAT_SPIR_V, moduleInfo.code.data(), sizeof(moduleInfo.code.front()) * moduleInfo.code.size());
        glSpecializeShader(shader, info.entryPoint.c_str(), constantIndex.size(), constantIndex.data(), constantValue.data());
        if (CheckShaderCompilation(shader)) //compilation is successful
        {
            glAttachShader(handle, shader);
            glLinkProgram(handle);
            glDetachShader(handle, shader);
        }
        glDeleteShader(shader);
    }, true);
}

Impl::~Impl()
{
    device.lock()->PushCommand([handle = handle] {
        glDeleteProgram(handle);
    }, false);
}
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
    return Handle(new Impl(a_Device, a_Info));
}
const Info& GetInfo(const Handle& a_Handle)
{
    return a_Handle->info;
}
}
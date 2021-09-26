/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 15:03:34
*/
#pragma once

#include <Handle.hpp>
#include <Pipeline/ShaderStage.hpp>
#include <Shader/Module.hpp>

#include <stdexcept>
#include <vector>

#include <GL/glew.h>

namespace OCRA::Pipeline::ShaderStage {
static inline auto CheckCompilation(GLuint stage)
{
    GLint result;
    glGetShaderiv(stage, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE) {
        GLsizei length { 0 };
        glGetShaderiv(stage, GL_INFO_LOG_LENGTH, &length);
        if (length > 1) {
            std::vector<char> infoLog(length, 0);
            glGetShaderInfoLog(stage, length, nullptr, infoLog.data());
            std::string logString(infoLog.begin(), infoLog.end());
            throw std::runtime_error(logString);
        } else
            throw std::runtime_error("Unknown Error");
    }
}
static inline auto GetGLStage(StageFlag& flags)
{
    //check if flags contains a certain stage & remove this stage fron flagset
    if (None != (flags & Vertex)) {
        flags = flags & ~Vertex;
        return GL_VERTEX_SHADER;
    }
    if (None != (flags & Geometry)) {
        flags = flags & ~Geometry;
        return GL_GEOMETRY_SHADER;
    }
    if (None != (flags & Fragment)) {
        flags = flags & ~Fragment;
        return GL_FRAGMENT_SHADER;
    }
    if (None != (flags & Compute)) {
        flags = flags & ~Compute;
        return GL_COMPUTE_SHADER;
    }
    if (None != (flags & TessControl)) {
        flags = flags & ~TessControl;
        return GL_TESS_CONTROL_SHADER;
    }
    if (None != (flags & TessEval)) {
        flags = flags & ~TessEval;
        return GL_TESS_EVALUATION_SHADER;
    }
    throw std::runtime_error("No Shader stage set");
    return GL_NONE;
}
//compiles the specified stages into a program
inline auto Compile(const Device::Handle& a_Device, const std::vector<Info>& a_Info)
{
    const auto program = glCreateProgram();
    std::vector<GLuint> stages;
    for (const auto& stageInfo : a_Info) {
        const auto& moduleInfo = Shader::Module::GetInfo(a_Device, stageInfo.module);
        auto stageFlags = stageInfo.stage;
        while (stageFlags != Pipeline::ShaderStage::None) {
            const auto& stage = glCreateShader(GetGLStage(stageFlags));
            const int codeSize = moduleInfo.code.size();
            const auto codeBuffer = moduleInfo.code.data();
            glShaderSource(stage, 1, (const char**)&codeBuffer, &codeSize);
            glCompileShader(stage);
            CheckCompilation(stage);
            glAttachShader(program, stage);
            stages.push_back(stage);
        }
    }
    glLinkProgram(program);
    for (const auto& stage : stages) {
        glDetachShader(program, stage);
        glDeleteShader(stage);
    }
    return program;
}
}
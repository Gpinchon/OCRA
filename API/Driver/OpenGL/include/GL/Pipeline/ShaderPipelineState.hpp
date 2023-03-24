/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 15:03:34
*/
#pragma once

#include <OCRA/Core.hpp>

#include <GL/ShaderStage.hpp>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Command::Buffer {
struct ExecutionState;
}

namespace OCRA::Pipeline {
//compiles the specified stages into a program pipeline
struct CompileShaderPipelineState {
    CompileShaderPipelineState(const Device::Handle& a_Device, const PipelineShaderPipelineState& a_Info, const PipelineDynamicState&);
    CompileShaderPipelineState(const CompileShaderPipelineState& a_Other);
    ~CompileShaderPipelineState();
    void operator()(Command::Buffer::ExecutionState&) const;
    const Device::WeakHandle device;
    const PipelineShaderPipelineState& info;
    std::vector<Shader::Stage> shaderStages;
    mutable uint32_t handle{ 0 };
};
}
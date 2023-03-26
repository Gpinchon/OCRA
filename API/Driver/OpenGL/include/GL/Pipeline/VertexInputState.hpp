/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-03 22:10:56
*/
#pragma once

#include <OCRA/Core.hpp>

#include <GL/Enums.hpp>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Command::Buffer {
struct ExecutionState;
}

namespace OCRA::Pipeline {
//compiles the specified Vertex Input State into a callback
//only the Vertex Attributes are compiled here, Vertex Bindings are compiled by on execution
struct CompileVertexInputState
{
    CompileVertexInputState(const Device::Handle& a_Device, const PipelineVertexInputState& a_Info, const PipelineDynamicState& a_DynamicState);
    CompileVertexInputState(const CompileVertexInputState& a_Other);
    ~CompileVertexInputState();
    void operator()(Command::Buffer::ExecutionState&) const;
    const bool applyVertexInputs;
    const uint32_t primitiveRestartIndex;
    const std::vector<VertexBindingDescription> bindingDescriptions;
    const Device::WeakHandle device;
    mutable uint32_t handle{ 0 };
};
}
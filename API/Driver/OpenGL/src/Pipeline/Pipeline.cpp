#include <OCRA/Command/Buffer.hpp>
#include <OCRA/Pipeline/Pipeline.hpp>

#include <GL/Device.hpp>
#include <GL/Pipeline/Pipeline.hpp>
#include <GL/Pipeline/Layout.hpp>
#include <GL/Command/ExecutionState.hpp>
#include <GL/Command/Buffer.hpp>

namespace OCRA::Command
{
void BindPipeline(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Pipeline::BindingPoint& a_BindingPoint,
    const OCRA::Pipeline::Handle& a_Pipeline)
{
    a_CommandBuffer->PushCommand([
        bindingPoint = uint8_t(a_BindingPoint),
        pipeline = a_Pipeline
    ](Buffer::ExecutionState& a_ExecutionState){
        a_ExecutionState.pipelineState.at(bindingPoint).pipeline = pipeline;
    });
}
}

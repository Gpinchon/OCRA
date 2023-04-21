#include <OCRA/Core.hpp>

#include <GL/Device.hpp>
#include <GL/Pipeline.hpp>
#include <GL/ExecutionState.hpp>
#include <GL/CommandBuffer.hpp>

namespace OCRA::Command
{
void BindPipeline(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Pipeline::Handle& a_Pipeline)
{
    a_CommandBuffer->PushCommand<GenericCommand>([
        bindingPoint = uint8_t(a_Pipeline->bindingPoint),
        pipeline = a_Pipeline
    ](Buffer::ExecutionState& a_ExecutionState){
        a_ExecutionState.pipelineState.at(bindingPoint).pipeline = pipeline;
    });
}
}

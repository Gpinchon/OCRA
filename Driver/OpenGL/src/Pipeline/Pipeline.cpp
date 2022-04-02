#include <Command/Buffer.hpp>
#include <Pipeline/Pipeline.hpp>

#include <GL/Command/ExecutionState.hpp>
#include <GL/Command/Buffer.hpp>

namespace OCRA::Command
{
void BindPipeline(
	const Command::Buffer::Handle& a_CommandBuffer,
	const OCRA::Pipeline::BindingPoint& a_BindingPoint,
	const OCRA::Pipeline::Handle& a_Pipeline)
{
	Command::Buffer::PushCommand(a_CommandBuffer, [
		bindingPoint = size_t(a_BindingPoint),
		pipeline = a_Pipeline
	](Buffer::ExecutionState& executionState){
		executionState.pipelineState.at(bindingPoint) = pipeline;
	});
}
}
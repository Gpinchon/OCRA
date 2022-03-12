#include <Command/Buffer.hpp>
#include <Pipeline/Pipeline.hpp>

#include <GL/Command/Buffer.hpp>

namespace OCRA::Command::Pipeline
{
void Bind(
	const Command::Buffer::Handle& a_CommandBuffer,
	const Pipeline::BindingPoint& a_BindingPoint,
	const Pipeline::Handle& a_Pipeline)
{
	Command::Buffer::PushCommand(a_CommandBuffer, [
		bindingPoint = size_t(a_BindingPoint),
		pipeline = a_Pipeline
	](Buffer::ExecutionState& executionState){
		executionState.pipelineState.at(bindingPoint) = pipeline;
	});
}
}

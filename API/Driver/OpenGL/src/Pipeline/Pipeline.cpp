#include <Command/Buffer.hpp>
#include <Pipeline/Pipeline.hpp>

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
		bindingPoint = size_t(a_BindingPoint),
		pipeline = a_Pipeline
	](Buffer::ExecutionState& executionState){
		executionState.pipelineState.at(bindingPoint) = pipeline;
	});
}
void PushConstants(
	const Command::Buffer::Handle& a_CommandBuffer,
	const Pipeline::Layout::Handle& a_PipelineLayout,
	const uint8_t a_Offset,
	const std::vector<char> a_Data)
{
	a_CommandBuffer->PushCommand([
		pipelineLayout = a_PipelineLayout,
		offset = size_t(a_Offset),
		data = a_Data](Buffer::ExecutionState& a_ExecutionState) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, pipelineLayout->pushConstantHandle);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER,
			a_ExecutionState.pushConstants.offset,
			a_ExecutionState.pushConstants.data.size(),
			a_ExecutionState.pushConstants.data.data());
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	});
}
}

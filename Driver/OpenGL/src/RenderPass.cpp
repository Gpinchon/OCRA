#include <RenderPass.hpp>

#include <GL/Command/ExecutionState.hpp>
#include <GL/Command/Buffer.hpp>

namespace OCRA::RenderPass
{
struct Impl {
	Info info;
};
}

namespace OCRA::Command
{
void BeginRenderPass(const Buffer::Handle& a_CommandBuffer, const RenderPassBeginInfo& a_BeginInfo, const SubPassContents& a_SubPassContents)
{
	Command::Buffer::PushCommand(a_CommandBuffer, [beginInfo = a_BeginInfo](Buffer::ExecutionState& executionState) {
		executionState.renderPass.renderPass = beginInfo.renderPass;
		executionState.renderPass.framebuffer = beginInfo.framebuffer;
		executionState.renderPass.renderArea = beginInfo.renderArea;
		executionState.renderPass.clearValues = beginInfo.clearValues;
	});
}
void EndRenderPass(const Buffer::Handle& a_CommandBuffer)
{
	Command::Buffer::PushCommand(a_CommandBuffer, [](Buffer::ExecutionState& executionState) {
		executionState.renderPass = {};
		executionState.subpassIndex = 0;
	});
}
}
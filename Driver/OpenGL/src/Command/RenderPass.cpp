#include <RenderPass.hpp>

#include <GL/Command/ExecutionState.hpp>
#include <GL/Command/Buffer.hpp>
#include <GL/FrameBuffer.hpp>

#include <GL/glew.h>

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
	assert(a_BeginInfo.framebuffer->info.renderPass == a_BeginInfo.renderPass); //check if FB & RP are compatible
	std::vector<GLenum> drawBuffers;
	std::vector<GLenum> readBuffers;
	std::vector<GLenum> clearBuffers;
	uint32_t bufferIndex = 0;
	for (const auto& attachment : info.attachments) {
		if (attachment.loadOp == LoadOperation::Load)
			readBuffers.push_back(GL_COLOR_ATTACHMENT0 + bufferIndex);
		else if (attachment.loadOp == LoadOperation::Clear)
			clearBuffers.push_back(GL_COLOR_ATTACHMENT0 + bufferIndex);
		if (attachment.loadOp == StoreOperation::Store)
			drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + bufferIndex);
	}
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
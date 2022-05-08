#include <RenderPass.hpp>
#include <Command/RenderPass.hpp>

#include <GL/Command/ExecutionState.hpp>
#include <GL/Command/Buffer.hpp>
#include <GL/FrameBuffer.hpp>

#include <GL/glew.h>

#include <cassert>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::RenderPass
{
struct Impl {
	Impl(const Device::Handle& a_Device, const Info& a_Info)
		: device(a_Device)
		, info(a_Info)
	{}
	Device::WeakHandle device;
	const Info info;
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
	return Handle(new Impl(a_Device, a_Info));
}
}

namespace OCRA::Command
{
void BeginRenderPass(const Buffer::Handle& a_CommandBuffer, const RenderPassBeginInfo& a_BeginInfo, const SubPassContents& a_SubPassContents)
{
	assert(a_BeginInfo.framebuffer->info.renderPass == a_BeginInfo.renderPass); //check if FB & RP are compatible
	a_CommandBuffer->PushCommand([beginInfo = a_BeginInfo](Buffer::ExecutionState& executionState) {
		executionState.renderPass.renderPass = beginInfo.renderPass;
		executionState.renderPass.framebuffer = beginInfo.framebuffer;
		executionState.renderPass.renderArea = beginInfo.renderArea;
		executionState.renderPass.clearValues = beginInfo.clearValues;
		executionState.renderPass.framebuffer->Bind();
		for (uint32_t i = 0; i < executionState.renderPass.clearValues.size(); ++i) {
			const auto& clearValue = executionState.renderPass.clearValues.at(i);
			const auto& attachment = executionState.renderPass.renderPass->info.attachments.at(i);
			if (attachment.loadOp == RenderPass::AttachmentDescription::LoadOperation::Clear) {
				//TODO really implement this
				glClearBufferfv(GL_COLOR, i, clearValue.color.float32);
			}
			if (attachment.storeOp == RenderPass::AttachmentDescription::StoreOperation::Store) {
				//TODO really implement this
				glColorMaski(i, true, true, true, true);
			}
			else if (attachment.storeOp == RenderPass::AttachmentDescription::StoreOperation::DontCare) {
				//TODO really implement this
				glColorMaski(i, false, false, false, false);
			}
		}
	});
}
void EndRenderPass(const Buffer::Handle& a_CommandBuffer)
{
	a_CommandBuffer->PushCommand([](Buffer::ExecutionState& executionState) {
		executionState.renderPass.framebuffer->Unbind();
		executionState.renderPass = {};
		executionState.subpassIndex = 0;
	});
}
}
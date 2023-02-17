////////////////////////////////////////////////////////////////////////////////
//  RenderPass commands
////////////////////////////////////////////////////////////////////////////////

#include <GL/Command/Buffer.hpp>

#include <GL/Common/Assert.hpp>
#include <GL/RenderPass.hpp>
#include <GL/FrameBuffer.hpp>

namespace OCRA::Command {
struct BeginRenderPassCommand : CommandI {
    BeginRenderPassCommand(
        const RenderPassBeginInfo& a_BeginInfo,
        const SubPassContents& a_SubPassContents)
        : beginInfo(a_BeginInfo)
        , subPassContent(a_SubPassContents)
    {
        OCRA_ASSERT(beginInfo.framebuffer->info.renderPass == beginInfo.renderPass); //check if FB & RP are compatible
    }
    virtual void operator()(Buffer::ExecutionState& a_ExecutionState) {
        a_ExecutionState.renderPass.renderPass = beginInfo.renderPass;
        a_ExecutionState.renderPass.framebuffer = beginInfo.framebuffer;
        a_ExecutionState.renderPass.renderArea = beginInfo.renderArea;
        a_ExecutionState.renderPass.colorClearValues = beginInfo.colorClearValues;
        a_ExecutionState.renderPass.depthClearValue = beginInfo.depthClearValue;
        a_ExecutionState.renderPass.stencilClearValue = beginInfo.stencilClearValue;
        a_ExecutionState.renderPass.framebuffer->Bind();
        a_ExecutionState.renderPass.renderPass->BeginRenderPass(a_ExecutionState);
    }
    const RenderPassBeginInfo beginInfo;
    const SubPassContents     subPassContent;
};

struct NextSubPassCommand : CommandI {
    NextSubPassCommand(const SubPassContents& a_SubPassContents)
        : subPassContents(a_SubPassContents)
    {}
    virtual void operator()(Buffer::ExecutionState& a_ExecutionState) {
        a_ExecutionState.subpassIndex++;
        a_ExecutionState.renderPass.renderPass->BeginSubPass(a_ExecutionState);
    }
    SubPassContents subPassContents;
};

struct EndRenderPassCommand : CommandI {
    virtual void operator()(Buffer::ExecutionState& a_ExecutionState) {
        a_ExecutionState.renderPass.framebuffer->Unbind();
        a_ExecutionState.renderPass = {};
    }
};
}

namespace OCRA::Command {
void BeginRenderPass(
    const Buffer::Handle&      a_CommandBuffer,
    const RenderPassBeginInfo& a_BeginInfo,
    const SubPassContents&     a_SubPassContents)
{
    a_CommandBuffer->PushCommand<BeginRenderPassCommand>(a_BeginInfo, a_SubPassContents);
}

void NextSubPass(
    const Command::Buffer::Handle& a_CommandBuffer,
    const SubPassContents&         a_SubPassContents)
{
    a_CommandBuffer->PushCommand<NextSubPassCommand>(a_SubPassContents);
}

void EndRenderPass(const Buffer::Handle& a_CommandBuffer)
{
    a_CommandBuffer->PushCommand<EndRenderPassCommand>();
}
}
////////////////////////////////////////////////////////////////////////////////
//  RenderPass commands
////////////////////////////////////////////////////////////////////////////////

#include <GL/CommandBuffer.hpp>

#include <GL/Common/Assert.hpp>
#include <GL/RenderPass.hpp>
#include <GL/FrameBuffer.hpp>

namespace OCRA::Command {
//struct BeginRenderPassCommand : CommandI {
//    BeginRenderPassCommand(
//        const RenderPassBeginInfo& a_BeginInfo,
//        const SubPassContents& a_SubPassContents)
//        : beginInfo(a_BeginInfo)
//        , subPassContent(a_SubPassContents)
//    {
//        OCRA_ASSERT(beginInfo.framebuffer->info.renderPass == beginInfo.renderPass); //check if FB & RP are compatible
//    }
//    virtual void operator()(Buffer::ExecutionState& a_ExecutionState) {
//        a_ExecutionState.renderPass.renderPass = beginInfo.renderPass;
//        a_ExecutionState.renderPass.framebuffer = beginInfo.framebuffer;
//        a_ExecutionState.renderPass.renderArea = beginInfo.renderArea;
//        a_ExecutionState.renderPass.colorClearValues  = beginInfo.colorClearValues;
//        a_ExecutionState.renderPass.depthClearValue   = beginInfo.depthStencilClearValue.depth;
//        a_ExecutionState.renderPass.stencilClearValue = beginInfo.depthStencilClearValue.stencil;
//        a_ExecutionState.renderPass.framebuffer->Bind();
//        a_ExecutionState.renderPass.renderPass->BeginRenderPass(a_ExecutionState);
//    }
//    const RenderPassBeginInfo beginInfo;
//    const SubPassContents     subPassContent;
//};
//
//struct NextSubPassCommand : CommandI {
//    NextSubPassCommand(const SubPassContents& a_SubPassContents)
//        : subPassContents(a_SubPassContents)
//    {}
//    virtual void operator()(Buffer::ExecutionState& a_ExecutionState) {
//        a_ExecutionState.subpassIndex++;
//        a_ExecutionState.renderPass.renderPass->BeginSubPass(a_ExecutionState);
//    }
//    SubPassContents subPassContents;
//};
//
//struct EndRenderPassCommand : CommandI {
//    virtual void operator()(Buffer::ExecutionState& a_ExecutionState) {
//        a_ExecutionState.renderPass.framebuffer->Unbind();
//        a_ExecutionState.renderPass = {};
//    }
//};
static inline void ClearAttachment(const RenderingAttachmentInfo& attachment, const Rect2D& area)
{
    if (attachment.loadOp != LoadOp::Clear || !attachment.imageView) return;
    auto type = GetGLClearColorType(attachment.imageView->info.format);
    glClearTexSubImage(
        attachment.imageView->handle,
        0,
        area.offset.x,     area.offset.y, 0,
        area.extent.width, area.extent.height, 0,
        attachment.imageView->format,
        type, &attachment.clearValue.color);
}
static inline void BindAttachment(const RenderingAttachmentInfo& attachment, GLenum attachmentPoint)
{
    auto handle = attachment.imageView ? attachment.imageView->handle : 0;
    glFramebufferTexture(
        GL_DRAW_FRAMEBUFFER, attachmentPoint,
        handle, 0);
}
struct BeginRenderingCommand : CommandI {
    BeginRenderingCommand(
        std::pmr::memory_resource* a_MemoryResource,
        const RenderingInfo& a_RenderingInfo)
        : info(a_RenderingInfo)
        , drawBuffers(a_MemoryResource)
    {
        drawBuffers.reserve(info.colorAttachments.size());
        for (auto i = 0u; i < info.colorAttachments.size(); ++i) {
            const auto& attachment = info.colorAttachments.at(i);
            if (attachment.storeOp == StoreOp::Store) {
                drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
            }
        }
    }
    virtual void operator()(Buffer::ExecutionState& a_ExecutionState) {
        for (auto i = 0u; i < info.colorAttachments.size(); ++i) {
            const auto& attachment = info.colorAttachments.at(i);
            ClearAttachment(attachment, info.area);
            BindAttachment(attachment, GL_COLOR_ATTACHMENT0 + i);
        }
        ClearAttachment(info.depthAttachment, info.area);
        BindAttachment(info.depthAttachment, GL_DEPTH_ATTACHMENT);
        ClearAttachment(info.stencilAttachment, info.area);
        BindAttachment(info.stencilAttachment, GL_STENCIL_ATTACHMENT);
        glViewport(
            info.area.offset.x, info.area.offset.y,
            info.area.extent.width, info.area.extent.height);
        glDrawBuffers(drawBuffers.size(), drawBuffers.data());
    }
    const RenderingInfo info;
    std::pmr::vector<GLenum> drawBuffers;
};
}

namespace OCRA::Command {
void BeginRendering(
    const Command::Buffer::Handle& a_CommandBuffer,
    const RenderingInfo& a_RenderingInfo)
{
    a_CommandBuffer->PushCommand<BeginRenderingCommand>(
        a_CommandBuffer->memoryResource, a_RenderingInfo);
}

void EndRendering(
    const Command::Buffer::Handle& a_CommandBuffer)
{}

//void BeginRenderPass(
//    const Buffer::Handle&      a_CommandBuffer,
//    const RenderPassBeginInfo& a_BeginInfo,
//    const SubPassContents&     a_SubPassContents)
//{
//    a_CommandBuffer->PushCommand<BeginRenderPassCommand>(a_BeginInfo, a_SubPassContents);
//}
//
//void NextSubPass(
//    const Command::Buffer::Handle& a_CommandBuffer,
//    const SubPassContents&         a_SubPassContents)
//{
//    a_CommandBuffer->PushCommand<NextSubPassCommand>(a_SubPassContents);
//}
//
//void EndRenderPass(const Buffer::Handle& a_CommandBuffer)
//{
//    a_CommandBuffer->PushCommand<EndRenderPassCommand>();
//}
}
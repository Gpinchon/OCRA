#include <RenderPass.hpp>
#include <Command/RenderPass.hpp>

#include <GL/Command/ExecutionState.hpp>
#include <GL/Command/Buffer.hpp>
#include <GL/FrameBuffer.hpp>

#include <GL/glew.h>

#include <vector>
#include <cassert>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::RenderPass
{
static inline auto GetClearOps(const Info& a_Info)
{
    std::vector<ClearBufferOp> clearOps;
    for (uint32_t i = 0; i < a_Info.colorAttachments.size(); ++i) {
        if (colorAttachment.loadOp == LoadOperation::Clear) {
            ClearBufferOp op;
            op.buffer = GL_COLOR;
            op.drawBuffer = i;
            clearOps.push_back(op);
        }
    }
    return clearOps;
}
static inline auto GetDrawBuffers(const Info& a_Info)
{
    std::vector<GLenum> drawBuffers;
    for (uint32_t i = 0; i < a_Info.colorAttachments.size(); ++i) {
        if (colorAttachment.storeOp == StoreOperation::Store) {
            if (colorAttachment.location == -1) drawBuffers.push_back(GL_NONE);
            else drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + colorAttachment.location);
        }
    }
    return drawBuffers;
}

Impl::Impl(const Device::Handle& a_Device, const Info& a_Info)
    : device(a_Device)
    , info(a_Info)
    , clearOps(GetClearOps(a_Info))
    , drawBuffers(GetDrawBuffers(a_Info))
{}

void Impl::BeginRenderPass(Buffer::ExecutionState& a_ExecutionState)
{
    const auto& renderPass = a_ExecutionState.renderPass;
    a_ExecutionState.framebuffer->Bind();
    glViewport(renderPass.renderArea.offset.x,     renderPass.renderArea.offset.y,
               renderPass.renderArea.extent.width, renderPass.renderArea.extent.height);
    if (info.depthAttachment.loadOP == LoadOperation::Clear)
        glClearBufferfv(GL_DEPTH, 0, &renderPass.depthClearValue);
    if (info.stencilAttachment.loadOP == LoadOperation::Clear)
        glClearBufferiv(GL_STENCIL, 0, &renderPass.stencilClearValue);
    for (uint32_t i = 0; i < clearOps.size(); ++i)
    {
        const auto& clearOp = clearOps.at(i);
        const auto& clearValue = renderPass.colorClearValues.at(i);
        //TODO use correct clearValue type
        glClearBufferfv(
            clearOp.buffer,
            clearOp.drawBuffer,
            clearValue.float32);
    }
    glDrawBuffers(drawBuffers.size(), drawBuffers.data());
    if (info.depthAttachment.storeOp == StoreOp::DontCare)
        glDepthMask(false);
    if (info.stencilAttachment.storeOp == StoreOp::DontCare)
        glStencilMask(0);
}

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
    a_CommandBuffer->PushCommand([beginInfo = a_BeginInfo](Buffer::ExecutionState& a_ExecutionState) {
        a_ExecutionState.subpassIndex = 0;
        a_ExecutionState.renderPass.renderPass = beginInfo.renderPass;
        a_ExecutionState.renderPass.framebuffer = beginInfo.framebuffer;
        a_ExecutionState.renderPass.renderArea = beginInfo.renderArea;
        a_ExecutionState.renderPass.colorClearValues = beginInfo.colorClearValues;
        a_ExecutionState.renderPass.depthClearValue = beginInfo.depthClearValue;
        a_ExecutionState.renderPass.stencilClearValue = beginInfo.stencilClearValue;
        a_ExecutionState.renderPass.renderPass->BeginRenderPass(a_ExecutionState);
    });
}
void EndRenderPass(const Buffer::Handle& a_CommandBuffer)
{
    a_CommandBuffer->PushCommand([](Buffer::ExecutionState& a_ExecutionState) {
        a_ExecutionState.renderPass.framebuffer->Unbind();
        a_ExecutionState.renderPass = {};
    });
}
}
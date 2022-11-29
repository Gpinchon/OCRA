#include <RenderPass.hpp>

#include <GL/FrameBuffer.hpp>
#include <GL/RenderPass.hpp>
#include <GL/Command/Buffer.hpp>
#include <GL/Command/ExecutionState.hpp>

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
        const auto& colorAttachment = a_Info.colorAttachments.at(i);
        if (colorAttachment.loadOp == LoadOperation::Clear) {
            ClearBufferOp op{};
            op.buffer = GL_COLOR;
            op.drawBuffer = i;
            clearOps.push_back(op);
        }
    }
    if (a_Info.depthAttachment.loadOp == LoadOperation::Clear) {
        ClearBufferOp op{};
        op.buffer = GL_DEPTH;
        op.drawBuffer = 0;
        clearOps.push_back(op);
    }
    if (a_Info.stencilAttachment.loadOp == LoadOperation::Clear) {
        ClearBufferOp op{};
        op.buffer = GL_STENCIL;
        op.drawBuffer = 0;
        clearOps.push_back(op);
    }
    return clearOps;
}

static inline auto GetSubPassDrawBuffers(const Info& a_Info, const SubPassDescription& a_Description)
{
    std::vector<GLenum> drawBuffers;
    for (uint32_t i = 0; i < a_Description.colorAttachments.size(); ++i) {
        const auto& renderPassColorAttachment = a_Info.colorAttachments.at(i);
        const auto& subPassColorAtachment = a_Description.colorAttachments.at(i);
        if (renderPassColorAttachment.storeOp == StoreOperation::Store) {
            if (subPassColorAtachment.location == -1) drawBuffers.push_back(uint32_t(GL_NONE));
            else drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + subPassColorAtachment.location);
        }
    }
    return drawBuffers;
}

static inline auto CreateSubPasses(const Info& a_Info)
{
    std::vector<SubPass> subPasses;
    for (const auto& subPass : a_Info.subPasses) {
        SubPass newSubPass{ GetSubPassDrawBuffers(a_Info, subPass) };
        subPasses.push_back(newSubPass);
    }
    return subPasses;
}

Impl::Impl(const Device::Handle& a_Device, const Info& a_Info)
    : device(a_Device)
    , info(a_Info)
    , clearOps(GetClearOps(a_Info))
    , subPasses(CreateSubPasses(a_Info))
{}

void Impl::BeginRenderPass(const Command::Buffer::ExecutionState& a_ExecutionState) const
{
    const auto& renderPass = a_ExecutionState.renderPass;
    glViewport(renderPass.renderArea.offset.x,     renderPass.renderArea.offset.y,
               renderPass.renderArea.extent.width, renderPass.renderArea.extent.height);
    if (info.depthAttachment.storeOp == StoreOperation::DontCare)
        glDepthMask(false);
    if (info.stencilAttachment.storeOp == StoreOperation::DontCare)
        glStencilMask(0);
}

void Impl::BeginSubPass(const Command::Buffer::ExecutionState& a_ExecutionState) const
{
    subPasses.at(a_ExecutionState.subpassIndex).Begin();
    ApplyClearOps(a_ExecutionState);
}

void Impl::ApplyClearOps(const Command::Buffer::ExecutionState& a_ExecutionState) const
{
    const auto& renderPass = a_ExecutionState.renderPass;
    if (info.depthAttachment.loadOp == LoadOperation::Clear)
        glClearBufferfv(GL_DEPTH, 0, &renderPass.depthClearValue);
    if (info.stencilAttachment.loadOp == LoadOperation::Clear)
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
}

void SubPass::Begin() const
{
    glTextureBarrier();
    glDrawBuffers(drawBuffers.size(), drawBuffers.data());
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
        a_ExecutionState.renderPass.renderPass = beginInfo.renderPass;
        a_ExecutionState.renderPass.framebuffer = beginInfo.framebuffer;
        a_ExecutionState.renderPass.renderArea = beginInfo.renderArea;
        a_ExecutionState.renderPass.colorClearValues = beginInfo.colorClearValues;
        a_ExecutionState.renderPass.depthClearValue = beginInfo.depthClearValue;
        a_ExecutionState.renderPass.stencilClearValue = beginInfo.stencilClearValue;
        a_ExecutionState.renderPass.framebuffer->Bind();
        a_ExecutionState.renderPass.renderPass->BeginRenderPass(a_ExecutionState);
    });
}
void NextSubPass(const Command::Buffer::Handle& a_CommandBuffer, const SubPassContents& a_SubPassContents)
{
    a_CommandBuffer->PushCommand([](Buffer::ExecutionState& a_ExecutionState) {
        a_ExecutionState.subpassIndex++;
        a_ExecutionState.renderPass.renderPass->BeginSubPass(a_ExecutionState);
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
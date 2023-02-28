#include <OCRA/Core.hpp>

#include <GL/FrameBuffer.hpp>
#include <GL/RenderPass.hpp>
#include <GL/Command/Buffer.hpp>
#include <GL/Command/ExecutionState.hpp>

#include <GL/glew.h>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Device
{
RenderPass::Handle CreateRenderPass(const Device::Handle& a_Device, const CreateRenderPassInfo& a_Info)
{
    return std::make_shared<RenderPass::Impl>(a_Device, a_Info);
}
}

namespace OCRA::RenderPass
{
static inline auto GetClearOps(const CreateRenderPassInfo& a_Info)
{
    std::vector<ClearBufferOp> clearOps;
    for (uint32_t i = 0; i < a_Info.colorAttachments.size(); ++i) {
        const auto& colorAttachment = a_Info.colorAttachments.at(i);
        if (colorAttachment.loadOp == LoadOp::Clear) {
            ClearBufferOp op{};
            op.buffer = GL_COLOR;
            op.drawBuffer = i;
            clearOps.push_back(op);
        }
    }
    if (a_Info.depthAttachment.loadOp == LoadOp::Clear) {
        ClearBufferOp op{};
        op.buffer = GL_DEPTH;
        op.drawBuffer = 0;
        clearOps.push_back(op);
    }
    if (a_Info.stencilAttachment.loadOp == LoadOp::Clear) {
        ClearBufferOp op{};
        op.buffer = GL_STENCIL;
        op.drawBuffer = 0;
        clearOps.push_back(op);
    }
    return clearOps;
}

static inline auto GetSubPassDrawBuffers(
    const CreateRenderPassInfo& a_Info,
    const SubPassDescription&   a_Description)
{
    std::vector<GLenum> drawBuffers;
    for (uint32_t i = 0; i < a_Description.colorAttachments.size(); ++i) {
        const auto& renderPassColorAttachment = a_Info.colorAttachments.at(i);
        const auto& subPassColorAtachment = a_Description.colorAttachments.at(i);
        if (renderPassColorAttachment.storeOp == StoreOp::Store) {
            if (subPassColorAtachment.location == -1) drawBuffers.push_back(uint32_t(GL_NONE));
            else drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + subPassColorAtachment.location);
        }
    }
    return drawBuffers;
}

static inline auto CreateSubPasses(const CreateRenderPassInfo& a_Info)
{
    std::vector<SubPass> subPasses;
    for (const auto& subPass : a_Info.subPasses) {
        SubPass newSubPass{ GetSubPassDrawBuffers(a_Info, subPass) };
        subPasses.push_back(newSubPass);
    }
    return subPasses;
}

Impl::Impl(const Device::Handle& a_Device, const CreateRenderPassInfo& a_Info)
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
    if (info.depthAttachment.storeOp == StoreOp::DontCare)
        glDepthMask(false);
    if (info.stencilAttachment.storeOp == StoreOp::DontCare)
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
    if (info.depthAttachment.loadOp == LoadOp::Clear)
        glClearBufferfv(GL_DEPTH, 0, &renderPass.depthClearValue);
    if (info.stencilAttachment.loadOp == LoadOp::Clear)
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
}

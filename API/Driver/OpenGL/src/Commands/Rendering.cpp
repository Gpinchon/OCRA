////////////////////////////////////////////////////////////////////////////////
//  Rendering commands
////////////////////////////////////////////////////////////////////////////////

#include <GL/CommandBuffer.hpp>
#include <GL/Common/Assert.hpp>
#include <GL/Device.hpp>

#include <iostream>

namespace OCRA::Command {
static inline void ClearAttachment(const RenderingAttachmentInfo& attachment, const Rect2D& area)
{
    if (attachment.loadOp != LoadOp::Clear || !attachment.imageView) return;
    auto format = GetGLDataFormat(attachment.imageView->info.format);
    auto type = GetGLClearColorType(attachment.imageView->info.format);
    glClearTexSubImage(
        attachment.imageView->handle,
        0,
        area.offset.x,     area.offset.y, 0,
        area.extent.width, area.extent.height, 1,
        format, type, &attachment.clearValue);
}
static inline void BindAttachment(const Image::View::Handle& imageView, uint32_t FB, GLenum attachmentPoint)
{
    auto handle = imageView ? imageView->handle : 0;
    glNamedFramebufferTexture(FB, attachmentPoint, handle, 0);
}
struct BeginRenderingCommand : CommandI {
    BeginRenderingCommand(
        std::pmr::memory_resource* a_MemoryResource,
        const Device::Handle& a_Device,
        const RenderingInfo& a_Info)
        : device(a_Device)
        , area(a_Info.area)
        , layerCount(a_Info.layerCount)
        , colorAttachments(a_Info.colorAttachments.begin(), a_Info.colorAttachments.end(), a_MemoryResource)
        , depthAttachment(a_Info.depthAttachment)
        , stencilAttachment(a_Info.stencilAttachment)
        , resolveMode(a_Info.resolveMode)
        , resolveAttachments(a_MemoryResource)
    {
        a_Device->PushCommand([this]() {
            glCreateFramebuffers(1, &FB);
            glNamedFramebufferParameteri(FB, GL_FRAMEBUFFER_DEFAULT_WIDTH, area.extent.width);
            glNamedFramebufferParameteri(FB, GL_FRAMEBUFFER_DEFAULT_HEIGHT, area.extent.height);
            glNamedFramebufferParameteri(FB, GL_FRAMEBUFFER_DEFAULT_LAYERS, layerCount);
            if (resolveMode != ResolveMode::None) {
                glCreateFramebuffers(1, &FBResolve);
                glNamedFramebufferParameteri(FBResolve, GL_FRAMEBUFFER_DEFAULT_WIDTH, area.extent.width);
                glNamedFramebufferParameteri(FBResolve, GL_FRAMEBUFFER_DEFAULT_HEIGHT, area.extent.height);
                glNamedFramebufferParameteri(FBResolve, GL_FRAMEBUFFER_DEFAULT_LAYERS, layerCount);
                drawAttachments.reserve(colorAttachments.size());
                resolveAttachments.reserve(colorAttachments.size());
            }
            for (auto i = 0u; i < colorAttachments.size(); ++i) {
                const auto& attachment = colorAttachments.at(i);
                if (attachment.storeOp == StoreOp::Store)
                    BindAttachment(attachment.imageView, FB, GL_COLOR_ATTACHMENT0 + i);
                if (resolveMode != ResolveMode::None) {
                    drawAttachments.push_back(GL_COLOR_ATTACHMENT0 + i);
                    if (attachment.resolve) {
                        BindAttachment(attachment.imageViewResolve, FBResolve, GL_COLOR_ATTACHMENT0 + i);
                        resolveAttachments.push_back(GL_COLOR_ATTACHMENT0 + i);
                    }
                }
            }
            if (depthAttachment.storeOp == StoreOp::Store) {
                BindAttachment(depthAttachment.imageView, FB, GL_DEPTH_ATTACHMENT);
                if (resolveMode != ResolveMode::None && depthAttachment.resolve) {
                    BindAttachment(depthAttachment.imageViewResolve, FBResolve, GL_DEPTH_ATTACHMENT);
                    resolveAttachments.push_back(GL_DEPTH_BUFFER_BIT);
                }
            }
            if (stencilAttachment.storeOp == StoreOp::Store) {
                BindAttachment(stencilAttachment.imageView, FB, GL_STENCIL_ATTACHMENT);
                if (resolveMode != ResolveMode::None && stencilAttachment.resolve) {
                    BindAttachment(stencilAttachment.imageViewResolve, FBResolve, GL_STENCIL_ATTACHMENT);
                    resolveAttachments.push_back(GL_STENCIL_BUFFER_BIT);
                }
            }
            OCRA_ASSERT(glCheckNamedFramebufferStatusEXT(FB, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        }, false);
    }
    virtual void operator()(Buffer::ExecutionState& a_ExecutionState) override {
        for (auto i = 0u; i < colorAttachments.size(); ++i) {
            const auto& attachment = colorAttachments.at(i);
            ClearAttachment(attachment, area);
        }
        ClearAttachment(depthAttachment, area);
        ClearAttachment(stencilAttachment, area);
        glScissor(
            area.offset.x, area.offset.y,
            area.extent.width, area.extent.height);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FB);
        if (resolveMode != ResolveMode::None) {
            a_ExecutionState.renderPass.frameBuffer = FB;
            a_ExecutionState.renderPass.frameBufferResolve = FBResolve;
            a_ExecutionState.renderPass.area = area;
            a_ExecutionState.renderPass.drawAttachments = { drawAttachments.begin(), drawAttachments.end() };
            a_ExecutionState.renderPass.resolveAttachments = { resolveAttachments.begin(), resolveAttachments.end() };
            a_ExecutionState.renderPass.resolveMode = resolveMode;
        }
    }
    const Device::WeakHandle device;
    const Rect2D   area;
    const uint32_t layerCount;
    const std::pmr::vector<RenderingAttachmentInfo> colorAttachments;
    const RenderingAttachmentInfo depthAttachment;
    const RenderingAttachmentInfo stencilAttachment;
    const ResolveMode resolveMode;
    std::pmr::vector<GLenum> drawAttachments;
    std::pmr::vector<GLenum> resolveAttachments;
    uint32_t FB = 0, FBResolve = 0;
};

struct EndRenderingCommand : CommandI {
    virtual void operator()(Buffer::ExecutionState& a_ExecutionState) override {
        const auto& renderPass = a_ExecutionState.renderPass;
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        if (renderPass.resolveMode != ResolveMode::None) {
            const auto& area = renderPass.area;
            for (auto& attachment : renderPass.resolveAttachments) {
                GLenum filter = GL_NONE;
                auto mode = renderPass.resolveMode == ResolveMode::Average ? GL_LINEAR : GL_NEAREST;
                if (attachment == GL_DEPTH_ATTACHMENT) filter = GL_DEPTH_BUFFER_BIT;
                else if (attachment == GL_STENCIL_ATTACHMENT) filter = GL_STENCIL_BUFFER_BIT;
                else filter = GL_COLOR_BUFFER_BIT;
                glNamedFramebufferReadBuffer(renderPass.frameBuffer, attachment);
                glNamedFramebufferDrawBuffer(renderPass.frameBufferResolve, attachment);
                glBlitNamedFramebuffer(
                    renderPass.frameBuffer,
                    renderPass.frameBufferResolve,
                    area.offset.x, area.offset.y,
                    area.offset.x + area.extent.width, area.offset.y + area.extent.height,
                    area.offset.x, area.offset.y,
                    area.offset.x + area.extent.width, area.offset.y + area.extent.height,
                    filter, mode);
            }
            glNamedFramebufferReadBuffer(renderPass.frameBuffer, GL_NONE);
            glNamedFramebufferDrawBuffers(renderPass.frameBuffer, renderPass.drawAttachments.size(), renderPass.drawAttachments.data());
        }
    }
};

}

namespace OCRA::Command {
void BeginRendering(
    const Command::Buffer::Handle& a_CommandBuffer,
    const RenderingInfo& a_RenderingInfo)
{
    a_CommandBuffer->PushCommand<BeginRenderingCommand>(
        a_CommandBuffer->memoryResource,
        a_CommandBuffer->device.lock(),
        a_RenderingInfo);
}

void EndRendering(
    const Command::Buffer::Handle& a_CommandBuffer)
{
    a_CommandBuffer->PushCommand<EndRenderingCommand>();
}
}
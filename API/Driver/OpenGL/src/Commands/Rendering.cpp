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
    if (attachment.loadOp != LoadOp::Clear || !attachment.imageView)
        return;
    auto format = GetGLDataFormat(attachment.imageView->info.format);
    auto type   = GetGLClearColorType(attachment.imageView->info.format);
    glClearTexSubImage(
        attachment.imageView->handle,
        0,
        area.offset.x, area.offset.y, 0,
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
        , flags(a_Info.flags)
        , area(a_Info.area)
        , layerCount(a_Info.layerCount)
        , colorAttachments(a_Info.colorAttachments.begin(), a_Info.colorAttachments.end(), a_MemoryResource)
        , depthAttachment(a_Info.depthAttachment)
        , stencilAttachment(a_Info.stencilAttachment)
        , resolveMode(a_Info.resolveMode)
        , resolveAttachments(a_MemoryResource)
    {
        if ((flags & RenderingFlagBits::Resuming) != 0)
            return;
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
                    if (attachment.imageViewResolve) {
                        BindAttachment(attachment.imageViewResolve, FBResolve, GL_COLOR_ATTACHMENT0 + i);
                        resolveAttachments.push_back(GL_COLOR_ATTACHMENT0 + i);
                    }
                }
            }
            if (depthAttachment.storeOp == StoreOp::Store) {
                BindAttachment(depthAttachment.imageView, FB, GL_DEPTH_ATTACHMENT);
                if (resolveMode != ResolveMode::None && depthAttachment.imageViewResolve) {
                    BindAttachment(depthAttachment.imageViewResolve, FBResolve, GL_DEPTH_ATTACHMENT);
                    resolveAttachments.push_back(GL_DEPTH_BUFFER_BIT);
                }
            }
            if (stencilAttachment.storeOp == StoreOp::Store) {
                BindAttachment(stencilAttachment.imageView, FB, GL_STENCIL_ATTACHMENT);
                if (resolveMode != ResolveMode::None && stencilAttachment.imageViewResolve) {
                    BindAttachment(stencilAttachment.imageViewResolve, FBResolve, GL_STENCIL_ATTACHMENT);
                    resolveAttachments.push_back(GL_STENCIL_BUFFER_BIT);
                }
            }
            OCRA_ASSERT(glCheckNamedFramebufferStatusEXT(FB, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
            OCRA_ASSERT(glCheckNamedFramebufferStatusEXT(FBResolve, GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        },
            false);
    }
    ~BeginRenderingCommand()
    {
        device.lock()->PushCommand([FB = FB, FBR = FBResolve] {
            // If this crashes, check your Begin/End sequences
            glDeleteFramebuffers(1, &FB);
            glDeleteFramebuffers(1, &FBR);
        },
            false);
    }
    virtual void operator()(Buffer::ExecutionState& a_ExecutionState) override
    {
        auto& renderPass = a_ExecutionState.renderPass;
        renderPass.flags = flags;
        if ((flags & RenderingFlagBits::Resuming) == 0) {
            renderPass.frameBuffer        = FB;
            renderPass.frameBufferResolve = FBResolve;
            renderPass.area               = area;
            renderPass.drawAttachments    = { drawAttachments.begin(), drawAttachments.end() };
            renderPass.resolveAttachments = { resolveAttachments.begin(), resolveAttachments.end() };
            renderPass.resolveMode        = resolveMode;
            for (auto i = 0u; i < colorAttachments.size(); ++i) {
                const auto& attachment = colorAttachments.at(i);
                ClearAttachment(attachment, area);
            }
            ClearAttachment(depthAttachment, area);
            ClearAttachment(stencilAttachment, area);
        }
        OCRA_ASSERT(renderPass.frameBuffer != 0 && "Invalid Render pass : No FrameBuffer bound");
        glScissor(
            renderPass.area.offset.x, renderPass.area.offset.y,
            renderPass.area.extent.width, renderPass.area.extent.height);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, renderPass.frameBuffer);
    }
    const Device::WeakHandle device;
    RenderingFlags flags;
    Rect2D area;
    uint32_t layerCount;
    std::pmr::vector<RenderingAttachmentInfo> colorAttachments;
    RenderingAttachmentInfo depthAttachment;
    RenderingAttachmentInfo stencilAttachment;
    ResolveMode resolveMode;
    std::pmr::vector<GLenum> drawAttachments;
    std::pmr::vector<GLenum> resolveAttachments;
    uint32_t FB = 0, FBResolve = 0;
};

struct EndRenderingCommand : CommandI {
    virtual void operator()(Buffer::ExecutionState& a_ExecutionState) override
    {
        auto& renderPass = a_ExecutionState.renderPass;
        OCRA_ASSERT(renderPass.frameBuffer != 0 && "Invalid Render pass : No FrameBuffer bound");
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        // Resolve & cleanup only if the pass is not being paused
        if ((renderPass.flags & RenderingFlagBits::Suspending) != 0)
            return;
        if (renderPass.resolveMode != ResolveMode::None) {
            OCRA_ASSERT(renderPass.frameBufferResolve != 0 && "Invalid Render pass : No resolve FrameBuffer bound");
            const auto& area = renderPass.area;
            for (auto& attachment : renderPass.resolveAttachments) {
                GLenum filter = GL_NONE;
                auto mode     = renderPass.resolveMode == ResolveMode::Average ? GL_LINEAR : GL_NEAREST;
                if (attachment == GL_DEPTH_ATTACHMENT)
                    filter = GL_DEPTH_BUFFER_BIT;
                else if (attachment == GL_STENCIL_ATTACHMENT)
                    filter = GL_STENCIL_BUFFER_BIT;
                else
                    filter = GL_COLOR_BUFFER_BIT;
                if (attachment != GL_DEPTH_ATTACHMENT && attachment != GL_STENCIL_ATTACHMENT) {
                    glNamedFramebufferReadBuffer(renderPass.frameBuffer, attachment);
                    glNamedFramebufferDrawBuffer(renderPass.frameBufferResolve, attachment);
                }
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
        renderPass.frameBuffer        = 0;
        renderPass.frameBufferResolve = 0;
        renderPass.area               = {};
        renderPass.drawAttachments.clear();
        renderPass.resolveAttachments.clear();
        renderPass.resolveMode = ResolveMode::None;
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

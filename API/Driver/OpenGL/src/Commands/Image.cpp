#include <GL/CommandBuffer.hpp>
#include <GL/Image.hpp>
#include <GL/Device.hpp>

namespace OCRA::Command
{
struct BlitImageCommand : CommandI {
    BlitImageCommand(
        std::pmr::memory_resource* a_MemoryResource,
        const Device::Handle& a_Device,
        const Image::Handle& a_SrcImage,
        const Image::Handle& a_DstImage,
        const size_t a_Count,
        const ImageBlit* a_Blits,
        const Filter& a_Filter)
        : device(a_Device)
        , srcImage(a_SrcImage)
        , dstImage(a_DstImage)
        , blits(a_Blits, a_Blits + a_Count, a_MemoryResource)
        , filter(a_Filter)
    {
        a_Device->PushCommand([this] {
            for (const auto& blit : blits) {
                for (auto blitLevel = blit.srcOffsets[0].z;
                    blitLevel < blit.srcOffsets[1].z;
                    ++blitLevel)
                {
                    std::array<GLuint, 2> FB{ 0, 0 };
                    glCreateFramebuffers(2, FB.data());
                    glNamedFramebufferReadBuffer(FB.at(0), GL_COLOR_ATTACHMENT0);
                    glNamedFramebufferDrawBuffer(FB.at(1), GL_COLOR_ATTACHMENT0);
                    if (dstImage->info.type == ImageType::Image1D) {
                        glNamedFramebufferTexture(
                            FB.at(0),
                            GL_COLOR_ATTACHMENT0,
                            dstImage->handle,
                            blitLevel);
                    }
                    else if (dstImage->info.type == ImageType::Image2D) {
                        //TODO manage multisampling
                        glNamedFramebufferTexture(
                            FB.at(0),
                            GL_COLOR_ATTACHMENT0,
                            dstImage->handle,
                            blitLevel);
                    }
                    else if (dstImage->info.type == ImageType::Image3D) {
                        glNamedFramebufferTextureLayer(
                            FB.at(0),
                            GL_COLOR_ATTACHMENT0,
                            dstImage->handle,
                            blitLevel, blitLevel);
                    }
                    if (srcImage->info.type == ImageType::Image1D) {
                        glNamedFramebufferTexture(
                            FB.at(1),
                            GL_COLOR_ATTACHMENT0,
                            srcImage->handle,
                            blitLevel);
                    }
                    else if (srcImage->info.type == ImageType::Image2D) {
                        //TODO manage multisampling
                        glNamedFramebufferTexture(
                            FB.at(1),
                            GL_COLOR_ATTACHMENT0,
                            srcImage->handle,
                            blitLevel);
                    }
                    else if (srcImage->info.type == ImageType::Image3D) {
                        glNamedFramebufferTextureLayer(
                            FB.at(1),
                            GL_COLOR_ATTACHMENT0,
                            srcImage->handle,
                            blitLevel, blitLevel);
                    }
                    framebuffers.push_back(FB);
                }
            }
        }, false);
    }
    ~BlitImageCommand() {
        device.lock()->PushCommand([framebuffers = framebuffers] {
            for (const auto &FB : framebuffers) {
                glDeleteFramebuffers(2, FB.data());
            }
        }, false);
    }
    virtual void operator()(Buffer::ExecutionState&) {
        for (auto blitIndex = 0u; blitIndex < blits.size(); ++blitIndex) {
            const auto& blit = blits.at(blitIndex);
            for (auto blitLayer = blit.srcOffsets[0].z;
                blitLayer < blit.srcOffsets[1].z;
                 ++blitLayer)
            {
                const auto& FB = framebuffers.at(blitIndex + blitLayer);
                glBlitNamedFramebuffer(
                    FB.at(0),
                    FB.at(1),
                    blit.srcOffsets[0].x, blit.srcOffsets[0].y,
                    blit.srcOffsets[1].x, blit.srcOffsets[1].y,
                    blit.dstOffsets[0].x, blit.dstOffsets[0].y,
                    blit.dstOffsets[1].x, blit.dstOffsets[1].y,
                    0, GL_NEAREST);
            }
        }
    }
    const Device::WeakHandle device;
    const Image::Handle srcImage;
    const Image::Handle dstImage;
    const std::pmr::vector<ImageBlit> blits;
    const Filter filter;
    std::pmr::vector<std::array<GLuint, 2>> framebuffers;
};
struct CopyImageCommand : CommandI {
    CopyImageCommand(
        std::pmr::memory_resource* a_MemoryResource,
        const Image::Handle& a_SrcImage,
        const Image::Handle& a_DstImage,
        const size_t& a_Count,
        const ImageCopy* a_Regions)
        : srcImage(a_SrcImage)
        , dstImage(a_DstImage)
        , regions(a_Regions, a_Regions + a_Count, a_MemoryResource)
    {}
    virtual void operator()(Buffer::ExecutionState&) {
        for (const auto& copy : regions) {
            glCopyImageSubData(
                srcImage->handle, srcImage->target, copy.srcSubresource.mipLevel, copy.srcOffset.x, copy.srcOffset.y, copy.srcOffset.z,
                dstImage->handle, dstImage->target, copy.dstSubresource.mipLevel, copy.dstOffset.x, copy.dstOffset.y, copy.dstOffset.z,
                copy.extent.width, copy.extent.height, copy.extent.depth);
        }
    }
    const Image::Handle srcImage;
    const Image::Handle dstImage;
    const std::pmr::vector<ImageCopy> regions;
};

struct GenerateMipMapCommand : CommandI {
    GenerateMipMapCommand(const Image::Handle& a_Image)
        : image(a_Image)
    {}
    virtual void operator()(Buffer::ExecutionState&) {
        glGenerateTextureMipmap(image->handle);
    }
    const Image::Handle image;
};

struct ClearColorImageCommand : CommandI {
    ClearColorImageCommand(
        std::pmr::memory_resource* a_MemoryResource,
        const Image::Handle& a_Image,
        const ColorValue&    a_Color,
        const size_t&        a_Count,
        const ImageSubresourceRange* a_Ranges)
        : image(a_Image)
        , color(a_Color)
        , ranges(a_Ranges, a_Ranges + a_Count, a_MemoryResource)
    {}
    virtual void operator()(Buffer::ExecutionState&) {
        for (const auto& range : ranges)
        {
            for (uint32_t level = range.baseMipLevel; level < range.levelCount; ++level)
                glClearTexImage(
                    image->handle,
                    level,
                    image->dataFormat,
                    GetGLClearColorType(image->info.format),
                    &color);
        }
    }
    const Image::Handle image;
    const ColorValue    color;
    const std::pmr::vector<ImageSubresourceRange> ranges;
};

void CopyBufferToImage(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Buffer::Handle&    a_SrcBuffer,
    const Image::Handle&           a_DstImage,
    const std::vector<ImageBufferCopy>& a_Regions)
{
    for (const auto& copy : a_Regions) CheckValidCopy(copy, a_DstImage);
    a_CommandBuffer->PushCommand<GenericCommand>([
        srcBuffer = a_SrcBuffer, dstImage = a_DstImage, regions = a_Regions
    ](Command::Buffer::ExecutionState&) {
        const auto& memoryBinding = srcBuffer->memoryBinding;
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, memoryBinding.memory->handle);
        for (auto& copy : regions) dstImage->Upload(copy, memoryBinding.offset);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    });
}

void CopyImageToBuffer(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Buffer::Handle&    a_DstBuffer,
    const Image::Handle&           a_SrcImage,
    const std::vector<ImageBufferCopy>& a_Regions)
{
    for (const auto& copy : a_Regions) CheckValidCopy(copy, a_SrcImage);
    a_CommandBuffer->PushCommand<GenericCommand>([
        dstBuffer = a_DstBuffer, srcImage = a_SrcImage, regions = a_Regions
    ](Command::Buffer::ExecutionState&) {
        const auto& memoryBinding = dstBuffer->memoryBinding;
        glBindBuffer(GL_PIXEL_PACK_BUFFER, memoryBinding.memory->handle);
        for (const auto& copy : regions)
            srcImage->Download(copy, memoryBinding.offset);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    });
}

void CopyImage(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const Image::Handle&            a_SrcImage,
    const Image::Handle&            a_DstImage,
    const std::vector<ImageCopy>&  a_Regions)
{
    a_CommandBuffer->PushCommand<CopyImageCommand>(
        a_CommandBuffer->memoryResource,
        a_SrcImage, a_DstImage,
        a_Regions.size(), a_Regions.data());
}

void GenerateMipMap(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Image::Handle& a_Image)
{
    a_CommandBuffer->PushCommand<GenerateMipMapCommand>(a_Image);
}

void ClearColorImage(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const Image::Handle&            a_Image,
    const ColorValue&               a_Color,
    const std::vector<ImageSubresourceRange>& a_Ranges)
{
    a_CommandBuffer->PushCommand<ClearColorImageCommand>(
        a_CommandBuffer->memoryResource, a_Image, a_Color,
        a_Ranges.size(), a_Ranges.data());
}

void TransitionImageLayout(
    const Command::Buffer::Handle& a_CommandBuffer,
    const ImageLayoutTransitionInfo& a_Transition)
{
    //TODO use glTextureBarrier, glMemoryBarrier and glMemoryBarrierByRegion when relevant
}

void TransitionImagesLayout(
    const Command::Buffer::Handle& a_CommandBuffer,
    const std::vector<ImageLayoutTransitionInfo>& a_Transitions)
{
    //TODO use glTextureBarrier, glMemoryBarrier and glMemoryBarrierByRegion when relevant
}

void BlitImage(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Image::Handle& a_SrcImage,
    const Image::Handle& a_DstImage,
    const std::vector<ImageBlit>& a_Blits,
    const Filter& a_Filter)
{
    //TODO implement a real blit with format conversion and all
    //std::vector<ImageCopy> copies(a_Blits.size());
    //std::transform(a_Blits.begin(), a_Blits.end(),
    //    copies.begin(), [](const ImageBlit& a_Blit) {
    //        ImageCopy copy;
    //        copy.srcOffset = a_Blit.srcOffsets[0];
    //        copy.dstOffset = a_Blit.dstOffsets[0];
    //        copy.srcSubresource = a_Blit.srcSubresource;
    //        copy.dstSubresource = a_Blit.dstSubresource;
    //        copy.extent.width  = a_Blit.srcOffsets[1].x;
    //        copy.extent.height = a_Blit.srcOffsets[1].y;
    //        copy.extent.depth  = a_Blit.srcOffsets[1].z;
    //        return copy;
    //    });
    //CopyImage(
    //    a_CommandBuffer,
    //    a_SrcImage,
    //    a_DstImage,
    //    copies);
    auto device = a_CommandBuffer->device.lock();
    a_CommandBuffer->PushCommand<GenericCommand>([
        device,
        srcImage = a_SrcImage, dstImage = a_DstImage,
        blits = a_Blits, filter = a_Filter
    ](Command::Buffer::ExecutionState& a_ExecutionState) {
        device->textureBlitter.Blit(srcImage, dstImage, blits, filter);
    });
}
}

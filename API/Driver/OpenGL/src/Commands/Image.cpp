#include <GL/Command/Buffer.hpp>
#include <GL/Image/Image.hpp>
#include <GL/Image/Format.hpp>

namespace OCRA::Command
{
void CopyBufferToImage(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Buffer::Handle&    a_SrcBuffer,
    const Image::Handle&           a_DstImage,
    const std::vector<Image::BufferCopy>& a_Regions)
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
    const std::vector<Image::BufferCopy>& a_Regions)
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
    const std::vector<Image::Copy>  a_Regions)
{
    a_CommandBuffer->PushCommand<GenericCommand>([
        srcImage = a_SrcImage, dstImage = a_DstImage, regions = a_Regions
    ](Command::Buffer::ExecutionState&) {
        for (const auto& copy : regions) {
            glCopyImageSubData(
                srcImage->handle, srcImage->target, copy.srcSubresource.level, copy.srcOffset.x, copy.srcOffset.y, copy.srcOffset.z,
                dstImage->handle, dstImage->target, copy.dstSubresource.level, copy.dstOffset.x, copy.dstOffset.y, copy.dstOffset.z,
                copy.extent.width, copy.extent.height, copy.extent.depth);
        }
    });
}

void GenerateMipMap(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Image::Handle& a_Image)
{
    a_CommandBuffer->PushCommand<GenericCommand>([image = a_Image](Command::Buffer::ExecutionState&) {
        glGenerateTextureMipmap(image->handle);
    });
}

void ClearColorImage(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Image::Handle&           a_Image,
    const Image::Layout&           a_ImageLayout,
    const ColorValue&              a_Color,
    const std::vector<Image::Subresource::Range>& a_Ranges)
{
    a_CommandBuffer->PushCommand<GenericCommand>([
        image = a_Image, clearColor = a_Color, ranges = a_Ranges
    ](Command::Buffer::ExecutionState&) {
        for (const auto& range : ranges)
        {
            for (uint32_t level = range.baseMipLevel; level < range.levelCount; ++level)
                glClearTexImage(
                        image->handle,
                        level,
                        image->dataFormat,
                        GetGLClearColorType(image->info.format),
                        &clearColor);
        }
    });
}
}

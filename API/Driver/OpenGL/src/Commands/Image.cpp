#include <GL/Command/Buffer.hpp>
#include <GL/Image/Image.hpp>
#include <GL/Image/Format.hpp>

namespace OCRA::Command
{
struct CopyImageCommand : CommandI {
    CopyImageCommand(
        std::pmr::memory_resource* a_MemoryResource,
        const Image::Handle& a_SrcImage,
        const Image::Handle& a_DstImage,
        const size_t& a_Count,
        const Image::Copy* a_Regions)
        : srcImage(a_SrcImage)
        , dstImage(a_DstImage)
        , regions(a_Regions, a_Regions + a_Count, a_MemoryResource)
    {}
    virtual void operator()(Buffer::ExecutionState&) {
        for (const auto& copy : regions) {
            glCopyImageSubData(
                srcImage->handle, srcImage->target, copy.srcSubresource.level, copy.srcOffset.x, copy.srcOffset.y, copy.srcOffset.z,
                dstImage->handle, dstImage->target, copy.dstSubresource.level, copy.dstOffset.x, copy.dstOffset.y, copy.dstOffset.z,
                copy.extent.width, copy.extent.height, copy.extent.depth);
        }
    }
    const Image::Handle srcImage;
    const Image::Handle dstImage;
    const std::pmr::vector<Image::Copy> regions;
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
        const Image::Subresource::Range* a_Ranges)
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
    const std::pmr::vector<Image::Subresource::Range> ranges;
};

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
    const std::vector<Image::Copy>&  a_Regions)
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
    const Command::Buffer::Handle& a_CommandBuffer,
    const Image::Handle&           a_Image,
    const Image::Layout&           a_ImageLayout,
    const ColorValue&              a_Color,
    const std::vector<Image::Subresource::Range>& a_Ranges)
{
    a_CommandBuffer->PushCommand<ClearColorImageCommand>(
        a_CommandBuffer->memoryResource, a_Image, a_Color,
        a_Ranges.size(), a_Ranges.data());
}
}

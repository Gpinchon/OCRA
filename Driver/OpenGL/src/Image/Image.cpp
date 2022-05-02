/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-07 18:55:03
*/
#include <Buffer.hpp>
#include <Handle.hpp>
#include <Image/Image.hpp>

#include <cassert>
#include <functional>
#include <stdexcept>
#include <vector>

#include <GL/Buffer.hpp>
#include <GL/Image/Image.hpp>
#include <GL/Image/Format.hpp>
#include <GL/Command/Buffer.hpp>
#include <GL/Memory.hpp>
#include <GL/Device.hpp>
#include <GL/Common/BufferOffset.hpp>
#include <GL/glew.h>

namespace OCRA::Image {
static inline auto CreateTexture(const Device::Handle& a_Device)
{
    uint32_t handle = 0;
    a_Device->PushCommand([&handle] {
        glGenTextures(1, &handle);
    }, true);
    return handle;
}
Impl::Impl(const Device::Handle& a_Device, const Info& a_Info)
    : device(a_Device)
    , info(a_Info)
    , internalFormat(GetGLSizedFormat(a_Info.format))
    , dataFormat(GetGLDataFormat(a_Info.format))
    , dataType(GetGLDataType(a_Info.format))
    , handle(CreateTexture(a_Device))
{}
Impl::~Impl() {
    device.lock()->PushCommand([handle = handle] {
        glDeleteTextures(1, &handle);
    }, false);
}

template<bool Compressed>
struct Texture : Impl {
    Texture(const Device::Handle& a_Device, const Info& a_Info, const GLenum& a_Target)
        : Impl(a_Device, a_Info)
        , target(a_Target)
    {
    }
    virtual void Download(const Command::BufferImageCopy& a_Copy, const size_t& a_MemoryOffset) override {
        if constexpr(Compressed) glGetCompressedTextureSubImage(
            handle,
            a_Copy.imageSubresource.level,
            a_Copy.imageOffset.x,
            a_Copy.imageOffset.y,
            a_Copy.imageOffset.z,
            a_Copy.imageExtent.width,
            a_Copy.imageExtent.height,
            a_Copy.imageExtent.depth,
            a_Copy.bufferRowLength * a_Copy.bufferImageHeight,
            BUFFER_OFFSET(a_Copy.bufferOffset + a_MemoryOffset));
        else glGetTextureSubImage(
            handle,
            a_Copy.imageSubresource.level,
            a_Copy.imageOffset.x,
            a_Copy.imageOffset.y,
            a_Copy.imageOffset.z,
            a_Copy.imageExtent.width,
            a_Copy.imageExtent.height,
            a_Copy.imageExtent.depth,
            dataFormat,
            dataType,
            a_Copy.bufferRowLength * a_Copy.bufferImageHeight,
            BUFFER_OFFSET(a_Copy.bufferOffset + a_MemoryOffset));
    };
    inline void Bind() const {
        glBindTexture(target, handle); //initialize texture object
    }
    inline void Unbind() const {
        glBindTexture(target, 0); //initialize texture object
    }
    const GLenum target;
};

template <bool Compressed>
struct Texture1D : Texture<Compressed>
{
    Texture1D(const Device::Handle& a_Device, const Info& a_Info)
        : Texture<Compressed>(a_Device, a_Info, GL_TEXTURE_1D)
    {
        if (info.samples == Samples::Samples1) {
            device.lock()->PushCommand([this] {
                Bind(); //initialize texture object
                glTexStorage1D(
                    target,
                    info.mipLevels,
                    internalFormat,
                    info.extent.width);
                Unbind(); //unbind texture
            }, true);
        }
        else throw std::runtime_error("Cannot create multisampled 1D textures");
    }
    virtual void Upload(const Command::BufferImageCopy& a_Copy, const size_t& a_MemoryOffset) override
    {
        Bind();
        if constexpr (Compressed)
            glCompressedTexSubImage1D(
                target,
                a_Copy.imageSubresource.level,
                a_Copy.imageOffset.x,
                a_Copy.imageExtent.width,
                internalFormat,
                a_Copy.bufferRowLength * a_Copy.bufferImageHeight,
                BUFFER_OFFSET(a_Copy.bufferOffset + a_MemoryOffset));
        else glTexSubImage1D(
                target,
                a_Copy.imageSubresource.level,
                a_Copy.imageOffset.x,
                a_Copy.imageExtent.width,
                dataFormat,
                dataType,
                BUFFER_OFFSET(a_Copy.bufferOffset + a_MemoryOffset));
        Unbind();
    }
};

template <bool Compressed>
struct Texture2D : Texture<Compressed>
{
    Texture2D(const Device::Handle& a_Device, const Info& a_Info)
        : Texture<Compressed>(a_Device, a_Info, a_Info.samples == Samples::Samples1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE)
    {
        device.lock()->PushCommand([this] {
            Bind();//initialize texture object
            if (info.samples == Samples::Samples1)
                glTexStorage2D(
                    target,
                    info.mipLevels,
                    internalFormat,
                    info.extent.width,
                    info.extent.height);
            else glTexStorage2DMultisample(
                target,
                uint32_t(info.samples),
                internalFormat,
                info.extent.width,
                info.extent.height,
                info.fixedSampleLocations);
            Unbind();
        }, true);
    }
    virtual void Upload(const Command::BufferImageCopy& a_Copy, const size_t& a_MemoryOffset) override
    {
        Bind();
        if constexpr (Compressed)
            glCompressedTexSubImage2D(
                target,
                a_Copy.imageSubresource.level,
                a_Copy.imageOffset.x,
                a_Copy.imageOffset.y,
                a_Copy.imageExtent.width,
                a_Copy.imageExtent.height,
                internalFormat,
                a_Copy.bufferRowLength * a_Copy.bufferImageHeight,
                BUFFER_OFFSET(a_Copy.bufferOffset + a_MemoryOffset));
        else glTexSubImage2D(
                target,
                a_Copy.imageSubresource.level,
                a_Copy.imageOffset.x,
                a_Copy.imageOffset.y,
                a_Copy.imageExtent.width,
                a_Copy.imageExtent.height,
                dataFormat,
                dataType,
                BUFFER_OFFSET(a_Copy.bufferOffset + a_MemoryOffset));
        Unbind();
    }
};

template <bool Compressed>
struct Texture3D : Texture<Compressed>
{
    Texture3D(const Device::Handle& a_Device, const Info& a_Info)
    : Texture<Compressed>(a_Device, a_Info, a_Info.samples == Samples::Samples1 ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D_MULTISAMPLE_ARRAY)
    {
        device.lock()->PushCommand([this] {
            Bind();//initialize texture object
            if (info.samples == Samples::Samples1)
                glTexStorage3D(
                    target,
                    info.mipLevels,
                    internalFormat,
                    info.extent.width,
                    info.extent.height,
                    info.extent.depth);
            else glTexStorage3DMultisample(
                target,
                uint32_t(info.samples),
                info.mipLevels,
                internalFormat,
                info.extent.width,
                info.extent.height,
                info.fixedSampleLocations);
            Unbind();
        }, true);
    }
    virtual void Upload(const Command::BufferImageCopy& a_Copy, const size_t& a_MemoryOffset) override
    {
        Bind();//initialize texture object
        if constexpr (Compressed)
            glCompressedTexSubImage3D(
                target,
                a_Copy.imageSubresource.level,
                a_Copy.imageOffset.x,
                a_Copy.imageOffset.y,
                a_Copy.imageOffset.z,
                a_Copy.imageExtent.width,
                a_Copy.imageExtent.height,
                a_Copy.imageExtent.depth,
                internalFormat,
                a_Copy.bufferRowLength * a_Copy.bufferImageHeight,
                BUFFER_OFFSET(a_Copy.bufferOffset + a_MemoryOffset));
        else glTexSubImage3D(
                target,
                a_Copy.imageSubresource.level,
                a_Copy.imageOffset.x,
                a_Copy.imageOffset.y,
                a_Copy.imageOffset.z,
                a_Copy.imageExtent.width,
                a_Copy.imageExtent.height,
                a_Copy.imageExtent.depth,
                dataFormat,
                dataType,
                BUFFER_OFFSET(a_Copy.bufferOffset + a_MemoryOffset));
        Unbind();
    }
};

Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
    Impl* impl;
    const auto isCompressed = IsCompressedFormat(a_Info.format);
    switch (a_Info.type)
    {
    case Image::Type::Image1D:
        if (isCompressed) impl = new Texture1D<true>(a_Device, a_Info);
        else impl = new Texture1D<false>(a_Device, a_Info);
        break;
    case Image::Type::Image2D:
        if (isCompressed) impl = new Texture2D<true>(a_Device, a_Info);
        else impl = new Texture2D<false>(a_Device, a_Info);
        break;
    case Image::Type::Image3D:
        if (isCompressed) impl = new Texture3D<true>(a_Device, a_Info);
        else impl = new Texture3D<false>(a_Device, a_Info);
        break;
    default: throw std::runtime_error("Unknown Image Type");
    }
    return Handle(impl);
}
const Info& GetInfo(const Device::Handle& a_Device, const Handle& a_Handle)
{
    return a_Handle->info;
}
unsigned GetGLHandle(const Device::Handle& a_Device, const Handle& a_Handle)
{
    return a_Handle->handle;
}
}

namespace OCRA::Command
{
static inline void CheckValidCopy(const BufferImageCopy& a_Copy, const Image::Handle& a_Image)
{
    assert(a_Copy.imageOffset.x + a_Copy.imageExtent.width  < a_Image->info.extent.width);
    assert(a_Copy.imageOffset.y + a_Copy.imageExtent.height < a_Image->info.extent.height);
    assert(a_Copy.imageOffset.z + a_Copy.imageExtent.depth  < a_Image->info.extent.depth);
    assert(a_Copy.imageSubresource.level < a_Image->info.mipLevels);
}
void CopyBufferToImage(
    const Command::Buffer::Handle& a_CommandBuffer,
    const OCRA::Buffer::Handle& a_SrcBuffer,
    const Image::Handle& a_DstImage,
    const std::vector<BufferImageCopy>& a_Regions)
{
    for (const auto& copy : a_Regions) CheckValidCopy(copy, a_DstImage);
    a_CommandBuffer->PushCommand([
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
    const OCRA::Buffer::Handle& a_DstBuffer,
    const Image::Handle& a_SrcImage,
    const std::vector<BufferImageCopy>& a_Regions)
{
    for (const auto& copy : a_Regions) CheckValidCopy(copy, a_SrcImage);
    a_CommandBuffer->PushCommand([
        dstBuffer = a_DstBuffer, srcImage = a_SrcImage, regions = a_Regions
    ](Command::Buffer::ExecutionState&) {
        const auto& memoryBinding = dstBuffer->memoryBinding;
        glBindBuffer(GL_PIXEL_PACK_BUFFER, memoryBinding.memory->handle);
        for (const auto& copy : regions)
            srcImage->Download(copy, memoryBinding.offset);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    });
}
void ClearColorImage(const Command::Buffer::Handle& a_CommandBuffer, const Image::Handle& a_Image, const Image::Layout& a_ImageLayout, const Image::ClearColor& a_Color, const std::vector<Image::Subresource::Range>& a_Ranges)
{
    //for (const auto& range : a_Ranges) CheckValidCopy(copy, a_SrcImage);
    a_CommandBuffer->PushCommand([
        image = a_Image, clearColor = a_Color, ranges = a_Ranges
    ](Command::Buffer::ExecutionState&) {
        for (const auto& range : ranges)
        {
            Image::ClearColor color{ 0, 0, 0, 0 };
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

/*
 * @Author: gpinchon
 * @Date:   2021-09-26 00:00:00
 * @Last Modified by:   gpinchon
 * @Last Modified time: 2021-10-07 18:55:03
 */
#include <OCRA/Core.hpp>

#include <GL/Buffer.hpp>
#include <GL/Common/Assert.hpp>
#include <GL/Common/BufferOffset.hpp>
#include <GL/Device.hpp>
#include <GL/Enums.hpp>
#include <GL/Image.hpp>
#include <GL/Memory.hpp>
#include <GL/glew.h>

#include <functional>
#include <stdexcept>
#include <vector>

namespace OCRA::Image {
void CheckValidCopy(const ImageBufferCopy& a_Copy, const Image::Handle& a_Image)
{
    OCRA_ASSERT(a_Copy.imageOffset.x + a_Copy.imageExtent.width <= a_Image->info.extent.width);
    OCRA_ASSERT(a_Copy.imageOffset.y + a_Copy.imageExtent.height <= a_Image->info.extent.height);
    OCRA_ASSERT(a_Copy.imageOffset.z + a_Copy.imageExtent.depth <= a_Image->info.extent.depth);
    OCRA_ASSERT(a_Copy.imageSubresource.mipLevel <= a_Image->info.mipLevels);
}
Impl::Impl(const Device::Handle& a_Device, const CreateImageInfo& a_Info, const uint32_t a_Target)
    : device(a_Device)
    , info(a_Info)
    , internalFormat(GetGLSizedFormat(a_Info.format))
    , dataFormat(GetGLDataFormat(a_Info.format))
    , dataType(GetGLDataType(a_Info.format))
    , target(a_Target)
{
    a_Device->PushCommand([this] {
        glCreateTextures(target, 1, &handle);
    },
        false);
}
Impl::~Impl()
{
    device.lock()->PushCommand([handle = handle] {
        glDeleteTextures(1, &handle);
    },
        false);
}

template <bool Compressed>
struct Texture : Impl {
    Texture(const Device::Handle& a_Device, const CreateImageInfo& a_Info, const GLenum& a_Target)
        : Impl(a_Device, a_Info, a_Target)
    {
    }
    virtual void Download(const ImageBufferCopy& a_Copy, const size_t& a_MemoryOffset) override;
    inline void Bind() const
    {
        glBindTexture(target, handle); // initialize texture object
    }
    inline void Unbind() const
    {
        glBindTexture(target, 0); // initialize texture object
    }
};

// compressed
void Texture<true>::Download(const ImageBufferCopy& a_Copy, const size_t& a_MemoryOffset)
{
    glGetCompressedTextureSubImage(
        handle,
        a_Copy.imageSubresource.mipLevel,
        a_Copy.imageOffset.x,
        a_Copy.imageOffset.y,
        a_Copy.imageOffset.z,
        a_Copy.imageExtent.width,
        a_Copy.imageExtent.height,
        a_Copy.imageExtent.depth,
        a_Copy.bufferRowLength * a_Copy.bufferImageHeight,
        BUFFER_OFFSET(a_Copy.bufferOffset + a_MemoryOffset));
}

// uncompressed
void Texture<false>::Download(const ImageBufferCopy& a_Copy, const size_t& a_MemoryOffset)
{
    glGetTextureSubImage(
        handle,
        a_Copy.imageSubresource.mipLevel,
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
}

template <bool Compressed>
struct Texture1D : Texture<Compressed> {
    Texture1D(const Device::Handle& a_Device, const CreateImageInfo& a_Info)
        : Texture<Compressed>(a_Device, a_Info, GL_TEXTURE_1D)
    {
        if (info.samples == SampleCount::Count1) {
            device.lock()->PushCommand([this] {
                glTextureStorage1D(
                    handle,
                    info.mipLevels,
                    internalFormat,
                    info.extent.width);
            },
                false);
        } else
            throw std::runtime_error("Cannot create multisampled 1D textures");
    }
    virtual void Upload(const ImageBufferCopy& a_Copy, const size_t& a_MemoryOffset) override;
};

// compressed
void Texture1D<true>::Upload(const ImageBufferCopy& a_Copy, const size_t& a_MemoryOffset)
{
    glCompressedTextureSubImage1D(
        handle,
        a_Copy.imageSubresource.mipLevel,
        a_Copy.imageOffset.x,
        a_Copy.imageExtent.width,
        internalFormat,
        a_Copy.bufferRowLength * a_Copy.bufferImageHeight,
        BUFFER_OFFSET(a_Copy.bufferOffset + a_MemoryOffset));
}

// uncompressed
void Texture1D<false>::Upload(const ImageBufferCopy& a_Copy, const size_t& a_MemoryOffset)
{
    glTextureSubImage1D(
        handle,
        a_Copy.imageSubresource.mipLevel,
        a_Copy.imageOffset.x,
        a_Copy.imageExtent.width,
        dataFormat,
        dataType,
        BUFFER_OFFSET(a_Copy.bufferOffset + a_MemoryOffset));
}

template <bool Compressed>
struct Texture2D : Texture<Compressed> {
    Texture2D(const Device::Handle& a_Device, const CreateImageInfo& a_Info)
        : Texture<Compressed>(a_Device, a_Info, a_Info.samples == SampleCount::Count1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE)
    {
        device.lock()->PushCommand([this] {
            if (info.samples == SampleCount::Count1)
                glTextureStorage2D(
                    handle,
                    info.mipLevels,
                    internalFormat,
                    info.extent.width,
                    info.extent.height);
            else
                glTextureStorage2DMultisample(
                    handle,
                    uint32_t(info.samples),
                    internalFormat,
                    info.extent.width,
                    info.extent.height,
                    info.fixedSampleLocations);
        },
            false);
    }
    virtual void Upload(const ImageBufferCopy& a_Copy, const size_t& a_MemoryOffset) override;
};

// compressed
void Texture2D<true>::Upload(const ImageBufferCopy& a_Copy, const size_t& a_MemoryOffset)
{
    glCompressedTextureSubImage2D(
        handle,
        a_Copy.imageSubresource.mipLevel,
        a_Copy.imageOffset.x,
        a_Copy.imageOffset.y,
        a_Copy.imageExtent.width,
        a_Copy.imageExtent.height,
        internalFormat,
        a_Copy.bufferRowLength * a_Copy.bufferImageHeight,
        BUFFER_OFFSET(a_Copy.bufferOffset + a_MemoryOffset));
}

// uncompressed
void Texture2D<false>::Upload(const ImageBufferCopy& a_Copy, const size_t& a_MemoryOffset)
{
    glTextureSubImage2D(
        handle,
        a_Copy.imageSubresource.mipLevel,
        a_Copy.imageOffset.x,
        a_Copy.imageOffset.y,
        a_Copy.imageExtent.width,
        a_Copy.imageExtent.height,
        dataFormat,
        dataType,
        BUFFER_OFFSET(a_Copy.bufferOffset + a_MemoryOffset));
}

template <bool Compressed>
struct Texture3D : Texture<Compressed> {
    Texture3D(const Device::Handle& a_Device, const CreateImageInfo& a_Info)
        : Texture<Compressed>(a_Device, a_Info, a_Info.samples == SampleCount::Count1 ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D_MULTISAMPLE_ARRAY)
    {
        device.lock()->PushCommand([this] {
            if (info.samples == SampleCount::Count1)
                glTextureStorage3D(
                    handle,
                    info.mipLevels,
                    internalFormat,
                    info.extent.width,
                    info.extent.height,
                    info.extent.depth);
            else
                glTextureStorage3DMultisample(
                    handle,
                    uint32_t(info.samples),
                    info.mipLevels,
                    internalFormat,
                    info.extent.width,
                    info.extent.height,
                    info.fixedSampleLocations);
            Unbind();
        },
            false);
    }
    virtual void Upload(const ImageBufferCopy& a_Copy, const size_t& a_MemoryOffset) override;
};

// compressed
void Texture3D<true>::Upload(const ImageBufferCopy& a_Copy, const size_t& a_MemoryOffset)
{
    glCompressedTextureSubImage3D(
        handle,
        a_Copy.imageSubresource.mipLevel,
        a_Copy.imageOffset.x,
        a_Copy.imageOffset.y,
        a_Copy.imageOffset.z,
        a_Copy.imageExtent.width,
        a_Copy.imageExtent.height,
        a_Copy.imageExtent.depth,
        internalFormat,
        a_Copy.bufferRowLength * a_Copy.bufferImageHeight,
        BUFFER_OFFSET(a_Copy.bufferOffset + a_MemoryOffset));
}

// uncompressed
void Texture3D<false>::Upload(const ImageBufferCopy& a_Copy, const size_t& a_MemoryOffset)
{
    glTextureSubImage3D(
        handle,
        a_Copy.imageSubresource.mipLevel,
        a_Copy.imageOffset.x,
        a_Copy.imageOffset.y,
        a_Copy.imageOffset.z,
        a_Copy.imageExtent.width,
        a_Copy.imageExtent.height,
        a_Copy.imageExtent.depth,
        dataFormat,
        dataType,
        BUFFER_OFFSET(a_Copy.bufferOffset + a_MemoryOffset));
}

void CopyBufferToImage(
    const OCRA::Buffer::Handle& a_SrcBuffer,
    const Image::Handle& a_DstImage,
    const std::vector<ImageBufferCopy>& a_Regions)
{
    for (const auto& copy : a_Regions)
        CheckValidCopy(copy, a_DstImage);
    a_SrcBuffer->device.lock()->PushCommand([srcBuffer = a_SrcBuffer, dstImage = a_DstImage, regions = a_Regions]() {
        const auto& memoryBinding = srcBuffer->memoryBinding;
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, memoryBinding.memory->handle);
        for (auto& copy : regions)
            dstImage->Upload(copy, memoryBinding.offset);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
    },
        false);
}

void CopyImageToBuffer(
    const OCRA::Buffer::Handle& a_DstBuffer,
    const Image::Handle& a_SrcImage,
    const std::vector<ImageBufferCopy>& a_Regions)
{
    for (const auto& copy : a_Regions)
        CheckValidCopy(copy, a_SrcImage);
    a_DstBuffer->device.lock()->PushCommand([dstBuffer = a_DstBuffer, srcImage = a_SrcImage, regions = a_Regions]() {
        const auto& memoryBinding = dstBuffer->memoryBinding;
        glBindBuffer(GL_PIXEL_PACK_BUFFER, memoryBinding.memory->handle);
        for (const auto& copy : regions)
            srcImage->Download(copy, memoryBinding.offset);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    },
        false);
}
void GenerateMipMap(
    const Device::Handle& a_Device,
    const Image::Handle& a_Image)
{
    a_Device->PushCommand([image = a_Image] {
        glGenerateTextureMipmap(image->handle);
    },
        false);
}

size_t GetDataSize(const Image::Handle& a_Image)
{
    const auto& info = a_Image->info;
    return GetPixelSize(info.format) * info.extent.width * info.extent.height * info.extent.depth;
}
}

namespace OCRA::Device {
Image::Handle CreateImage(
    const Device::Handle& a_Device,
    const CreateImageInfo& a_Info)
{
    Image::Impl* impl;
    const auto isCompressed = IsCompressedFormat(a_Info.format);
    switch (a_Info.type) {
    case ImageType::Image1D:
        if (isCompressed)
            impl = new Image::Texture1D<true>(a_Device, a_Info);
        else
            impl = new Image::Texture1D<false>(a_Device, a_Info);
        break;
    case ImageType::Image2D:
        if (isCompressed)
            impl = new Image::Texture2D<true>(a_Device, a_Info);
        else
            impl = new Image::Texture2D<false>(a_Device, a_Info);
        break;
    case ImageType::Image3D:
        if (isCompressed)
            impl = new Image::Texture3D<true>(a_Device, a_Info);
        else
            impl = new Image::Texture3D<false>(a_Device, a_Info);
        break;
    default:
        throw std::runtime_error("Unknown Image Type");
    }
    return Image::Handle(impl);
}
}

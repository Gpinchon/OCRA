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
#include <GL/Image/Format.hpp>
#include <GL/glew.h>
#include <GL/Command/Buffer.hpp>
#include <GL/Memory.hpp>

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

namespace OCRA::Image {
struct Impl
{
    Impl(const Device::Handle& a_Device, const Info& a_Info)
        : info(a_Info)
        , internalFormat(GetGLSizedFormat(a_Info.format))
        , dataFormat(GetGLDataFormat(a_Info.format))
        , dataType(GetGLDataType(a_Info.format))
    {
        glGenTextures(1, &handle);
    }
    ~Impl() {
        glDeleteTextures(1, &handle);
    }
    virtual void Download(const Command::BufferImageCopy& a_Copy, const size_t& a_MemoryOffset) = 0;
    virtual void Upload(const Command::BufferImageCopy& a_Copy, const size_t& a_MemoryOffset) = 0;
    const Info info;
    const GLenum internalFormat;
    const GLenum dataType;
    const GLenum dataFormat;
    GLuint handle{ 0 };
};

template<bool Compressed>
struct Texture : Impl {
    Texture(const Device::Handle& a_Device, const Info& a_Info, const GLenum& a_Target)
        : Impl(a_Device, a_Info)
        , target(a_Target)
    {}
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
        if (info.samples == Info::Samples::Samples1) {
            Bind(); //initialize texture object
            glTexStorage1D(
                target,
                info.mipLevels,
                internalFormat,
                info.extent.width);
            Unbind(); //unbind texture
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
        : Texture<Compressed>(a_Device, a_Info, a_Info.samples == Info::Samples::Samples1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE)
    {
        Bind();//initialize texture object
        if (info.samples == Info::Samples::Samples1)
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
    : Texture<Compressed>(a_Device, a_Info, a_Info.samples == Info::Samples::Samples1 ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D_MULTISAMPLE_ARRAY)
    {
        Bind();//initialize texture object
        if (info.samples == Info::Samples::Samples1)
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
    case Image::Info::Type::Image1D:
        if (isCompressed) impl = new Texture1D<true>(a_Device, a_Info);
        else impl = new Texture1D<false>(a_Device, a_Info);
        break;
    case Image::Info::Type::Image2D:
        if (isCompressed) impl = new Texture2D<true>(a_Device, a_Info);
        else impl = new Texture2D<false>(a_Device, a_Info);
        break;
    case Image::Info::Type::Image3D:
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
    Command::Buffer::PushCommand(a_CommandBuffer, [
        srcBuffer = a_SrcBuffer, dstImage = a_DstImage, regions = a_Regions
    ](Command::Buffer::ExecutionState&) {
        auto& memoryBinding = OCRA::Buffer::GetMemoryBinding(srcBuffer);
        glBindBuffer(GL_PIXEL_UNPACK_BUFFER, Memory::GetGLHandle(memoryBinding.memory));
        for (auto& copy : regions) dstImage->Upload(copy, memoryBinding.memoryOffset);
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
    Command::Buffer::PushCommand(a_CommandBuffer, [
        dstBuffer = a_DstBuffer, srcImage = a_SrcImage, regions = a_Regions
    ](Command::Buffer::ExecutionState&) {
        auto& memoryBinding = OCRA::Buffer::GetMemoryBinding(dstBuffer);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, Memory::GetGLHandle(memoryBinding.memory));
        for (const auto& copy : regions)
            srcImage->Download(copy, memoryBinding.memoryOffset);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    });
}
}

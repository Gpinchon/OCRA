/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-07 18:55:03
*/
#include <Handle.hpp>
#include <Image/Image.hpp>
#include <Command/Image.hpp>

#include <cassert>
#include <functional>
#include <map>
#include <stdexcept>
#include <vector>

#include <GL/Buffer/Buffer.hpp>
#include <GL/Buffer/Transfer.hpp>
#include <GL/Image/Format.hpp>
#include <GL/glew.h>

namespace OCRA::Image {
struct Impl;
using TransferFunction = std::function<void(const Device::Handle&, const Command::BufferImageCopy&, const Impl&)>;

extern TransferFunction CompressedDownload;
extern TransferFunction UncompressedDownload;

extern TransferFunction Compressed1DUpload;
extern TransferFunction Uncompressed1DUpload;

extern TransferFunction Compressed2DUpload;
extern TransferFunction Uncompressed2DUpload;

extern TransferFunction Compressed3DUpload;
extern TransferFunction Uncompressed3DUpload;

struct Impl {
    Impl(const Device::Handle& a_Device, const Info& a_Info)
        : info(a_Info)
        , internalFormat(GetGLSizedFormat(a_Info.format))
        , dataFormat(GetGLDataFormat(a_Info.format))
        , dataType(GetGLDataType(a_Info.format))
    {
        glGenTextures(1, &handle);
        auto isCompressed { IsCompressedFormat(a_Info.format) };
        Download = isCompressed ? CompressedDownload : UncompressedDownload;
        switch (a_Info.type)
        {
        case Image::Info::Type::Image1D:
            Upload = isCompressed ? Compressed1DUpload : Uncompressed1DUpload;
            break;
        case Image::Info::Type::Image2D:
            Upload = isCompressed ? Compressed2DUpload : Uncompressed2DUpload;
            break;
        case Image::Info::Type::Image3D:
            Upload = isCompressed ? Compressed3DUpload : Uncompressed3DUpload;
            break;
        }
        if (a_Info.samples == Info::Samples::Samples1) {
            switch (a_Info.type) {
            case Image::Info::Type::Image1D:
                glTextureStorage1D(
                    handle,
                    a_Info.mipLevels,
                    internalFormat,
                    a_Info.extent.width);
                break;
            case Image::Info::Type::Image2D:
                glTextureStorage2D(
                    handle,
                    a_Info.mipLevels,
                    internalFormat,
                    a_Info.extent.width,
                    a_Info.extent.height);
                break;
            case Image::Info::Type::Image3D:
                glTextureStorage3D(
                    handle,
                    a_Info.mipLevels,
                    internalFormat,
                    a_Info.extent.width,
                    a_Info.extent.height,
                    a_Info.extent.depth);
                break;
            default:
                throw std::runtime_error("Unknown Image Type");
            }
        } else {
            switch (a_Info.type) {
            case Image::Info::Type::Image1D:
                throw std::runtime_error("Cannot create multisampled 1D textures");
                break;
            case Image::Info::Type::Image2D:
                glTextureStorage2DMultisample(
                    handle,
                    Uint32(a_Info.samples),
                    internalFormat,
                    a_Info.extent.width,
                    a_Info.extent.height,
                    a_Info.fixedSampleLocations);
                break;
            case Image::Info::Type::Image3D:
                glTextureStorage3DMultisample(
                    handle,
                    Uint32(a_Info.samples),
                    a_Info.mipLevels,
                    internalFormat,
                    a_Info.extent.width,
                    a_Info.extent.height,
                    a_Info.fixedSampleLocations);
                break;
            default:
                throw std::runtime_error("Unknown multisampled Image Type");
            }
        }
    }
    ~Impl()
    {
        glDeleteTextures(1, &handle);
    }
    GLuint handle { 0 };
    const GLenum internalFormat;
    const GLenum dataType;
    const GLenum dataFormat;
    const Info info;
    TransferFunction Upload;
    TransferFunction Download;
};

static Handle s_CurrentHandle = 0;
static std::map<Handle, Impl> s_Images;
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
    ++s_CurrentHandle;
    s_Images.emplace(s_CurrentHandle, Impl(a_Device, a_Info));
    return s_CurrentHandle;
}
auto& Get(const Handle& a_Handle)
{
    return s_Images.at(a_Handle);
}
void Destroy(const Device::Handle& a_Device, const Handle& a_Handle)
{
    s_Images.erase(a_Handle);
}
const Info& GetInfo(const Device::Handle& a_Device, const Handle& a_Handle)
{
    return s_Images.at(a_Handle).info;
}
unsigned GetGLHandle(const Device::Handle& a_Device, const Handle& a_Handle)
{
    return s_Images.at(a_Handle).handle;
}
}

namespace OCRA::Command
{
void CheckValidCopy(const BufferImageCopy& a_Copy, const Image::Impl& a_Image)
{
    assert(a_Copy.imageOffset.x + a_Copy.imageExtent.width  < a_Image.info.extent.width);
    assert(a_Copy.imageOffset.y + a_Copy.imageExtent.height < a_Image.info.extent.height);
    assert(a_Copy.imageOffset.z + a_Copy.imageExtent.depth  < a_Image.info.extent.depth);
    assert(a_Copy.imageSubresource.level < a_Image.info.mipLevels);
}
void CopyBufferToImage(
    const Device::Handle& a_Device,
    const OCRA::Buffer::Transfer::Handle& a_SrcBuffer,
    const Image::Handle& a_DstImage,
    const std::vector<Command::BufferImageCopy>& a_Regions)
{
    auto &image = Image::Get(a_DstImage);
    auto bufferHandle{ OCRA::Buffer::Transfer::GetBufferHandle(a_SrcBuffer) };
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, OCRA::Buffer::GetGLHandle(bufferHandle));
    for (const auto& copy : a_Regions) {
        CheckValidCopy(copy, image);
        image.Upload(a_Device, copy, image);
    }
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

void CopyImageToBuffer(
    const Device::Handle& a_Device,
    const OCRA::Buffer::Transfer::Handle& a_DstBuffer,
    const Image::Handle& a_SrcImage,
    const std::vector<Command::BufferImageCopy>& a_Regions)
{
    auto& image = Image::Get(a_SrcImage);
    auto bufferHandle{ OCRA::Buffer::Transfer::GetBufferHandle(a_DstBuffer) };
    glBindBuffer(GL_PIXEL_PACK_BUFFER, OCRA::Buffer::GetGLHandle(bufferHandle));
    for (const auto& copy : a_Regions) {
        CheckValidCopy(copy, image);
        image.Download(a_Device, copy, image);
    }
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
}
}

#define BUFFER_OFFSET(i) ((char*)NULL + (i))

namespace OCRA::Image {
OCRA::Image::TransferFunction OCRA::Image::UncompressedDownload = [](const Device::Handle& a_Device, const Command::BufferImageCopy& a_Copy, const Impl& a_Image) {
    glGetTextureSubImage(
        a_Image.handle,
        a_Copy.imageSubresource.level,
        a_Copy.imageOffset.x,
        a_Copy.imageOffset.y,
        a_Copy.imageOffset.z,
        a_Copy.imageExtent.width,
        a_Copy.imageExtent.height,
        a_Copy.imageExtent.depth,
        a_Image.dataFormat,
        a_Image.dataType,
        a_Copy.bufferRowLength * a_Copy.bufferImageHeight,
        BUFFER_OFFSET(a_Copy.bufferOffset));
};

OCRA::Image::TransferFunction OCRA::Image::CompressedDownload = [](const Device::Handle& a_Device, const Command::BufferImageCopy& a_Copy, const Impl& a_Image) {
    glGetCompressedTextureSubImage(
        a_Image.handle,
        a_Copy.imageSubresource.level,
        a_Copy.imageOffset.x,
        a_Copy.imageOffset.y,
        a_Copy.imageOffset.z,
        a_Copy.imageExtent.width,
        a_Copy.imageExtent.height,
        a_Copy.imageExtent.depth,
        a_Copy.bufferRowLength * a_Copy.bufferImageHeight,
        BUFFER_OFFSET(a_Copy.bufferOffset));
};

OCRA::Image::TransferFunction OCRA::Image::Compressed1DUpload = [](const Device::Handle& a_Device, const Command::BufferImageCopy& a_Copy, const Impl& a_Image) {
    glCompressedTextureSubImage1D(
        a_Image.handle,
        a_Copy.imageSubresource.level,
        a_Copy.imageOffset.x,
        a_Copy.imageExtent.width,
        a_Image.internalFormat,
        a_Copy.bufferRowLength * a_Copy.bufferImageHeight,
        BUFFER_OFFSET(a_Copy.bufferOffset));
};

OCRA::Image::TransferFunction OCRA::Image::Compressed2DUpload = [](const Device::Handle& a_Device, const Command::BufferImageCopy& a_Copy, const Impl& a_Image) {
    glCompressedTextureSubImage2D(
        a_Image.handle,
        a_Copy.imageSubresource.level,
        a_Copy.imageOffset.x,
        a_Copy.imageOffset.y,
        a_Copy.imageExtent.width,
        a_Copy.imageExtent.height,
        a_Image.internalFormat,
        a_Copy.bufferRowLength * a_Copy.bufferImageHeight,
        BUFFER_OFFSET(a_Copy.bufferOffset));
};

OCRA::Image::TransferFunction OCRA::Image::Compressed3DUpload = [](const Device::Handle& a_Device, const Command::BufferImageCopy& a_Copy, const Impl& a_Image) {
    glCompressedTextureSubImage3D(
        a_Image.handle,
        a_Copy.imageSubresource.level,
        a_Copy.imageOffset.x,
        a_Copy.imageOffset.y,
        a_Copy.imageOffset.z,
        a_Copy.imageExtent.width,
        a_Copy.imageExtent.height,
        a_Copy.imageExtent.depth,
        a_Image.internalFormat,
        a_Copy.bufferRowLength * a_Copy.bufferImageHeight,
        BUFFER_OFFSET(a_Copy.bufferOffset));
};

OCRA::Image::TransferFunction OCRA::Image::Uncompressed1DUpload = [](const Device::Handle& a_Device, const Command::BufferImageCopy& a_Copy, const Impl& a_Image) {
    glTextureSubImage1D(
        a_Image.handle,
        a_Copy.imageSubresource.level,
        a_Copy.imageOffset.x,
        a_Copy.imageExtent.width,
        a_Image.dataFormat,
        a_Image.dataType,
        BUFFER_OFFSET(a_Copy.bufferOffset));
};

OCRA::Image::TransferFunction OCRA::Image::Uncompressed2DUpload = [](const Device::Handle& a_Device, const Command::BufferImageCopy& a_Copy, const Impl& a_Image) {
        glTextureSubImage2D(
            a_Image.handle,
            a_Copy.imageSubresource.level,
            a_Copy.imageOffset.x,
            a_Copy.imageOffset.y,
            a_Copy.imageExtent.width,
            a_Copy.imageExtent.height,
            a_Image.dataFormat,
            a_Image.dataType,
            BUFFER_OFFSET(a_Copy.bufferOffset));
};

OCRA::Image::TransferFunction OCRA::Image::Uncompressed3DUpload = [](const Device::Handle& a_Device, const Command::BufferImageCopy& a_Copy, const Impl& a_Image) {
    glTextureSubImage3D(
        a_Image.handle,
        a_Copy.imageSubresource.level,
        a_Copy.imageOffset.x,
        a_Copy.imageOffset.y,
        a_Copy.imageOffset.z,
        a_Copy.imageExtent.width,
        a_Copy.imageExtent.height,
        a_Copy.imageExtent.depth,
        a_Image.dataFormat,
        a_Image.dataType,
        BUFFER_OFFSET(a_Copy.bufferOffset));
};
}
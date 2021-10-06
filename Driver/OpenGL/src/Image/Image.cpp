/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-04 20:32:43
*/
#include <Handle.hpp>
#include <Image/Image.hpp>
#include <Image/Operations.hpp>

#include <map>
#include <stdexcept>
#include <vector>
#include <cassert>

#include <GL/glew.h>
#include <GL/Image/Format.hpp>
#include <GL/Buffer/Transfer.hpp>

namespace OCRA::Image {
struct Impl {
    Impl(const Device::Handle& a_Device, const Info& a_Info)
        : info(a_Info)
        , internalFormat(GetGLSizedFormat(a_Info.format))
    {
        glGenTextures(1, &handle);
        ;
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
    const Info info;
};
static Handle s_CurrentHandle = 0;
static std::map<Handle, Impl> s_Images;
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
    ++s_CurrentHandle;
    s_Images.emplace(s_CurrentHandle, Impl(a_Device, a_Info));
    return s_CurrentHandle;
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
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void ReadFromBuffer(
    const Device::Handle& a_Device,
    const Buffer::Transfer::Handle& a_srcBuffer,
    const Image::Handle& a_dstImage,
    const std::vector<BufferCopy>& a_Regions)
{
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, Buffer::Transfer::GetGLHandle(a_Device, a_srcBuffer));
    const auto &image{ s_Images.at(a_dstImage) };
    for (const auto& copy : a_Regions) {
        assert(copy.imageOffset.x < image.info.extent.width);
        assert(copy.imageOffset.y < image.info.extent.height);
        glTextureSubImage2D(
            image.handle,
            copy.imageSubresource.mipLevel,
            copy.imageOffset.x,
            copy.imageOffset.y,
            copy.imageExtent.width,
            copy.imageExtent.height,
            image.internalFormat,
            GLenum type,
            BUFFER_OFFSET(copy.bufferOffset));
    }
    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}
}
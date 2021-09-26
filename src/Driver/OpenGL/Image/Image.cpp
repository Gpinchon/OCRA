/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:31:47
*/
#include <Driver/OpenGL/Image/Format.hpp>
#include <Handle.hpp>
#include <Image/Image.hpp>

#include <map>
#include <stdexcept>

#include <GL/glew.h>

namespace OCRA::Image {
struct Impl {
    ~Impl()
    {
        glDeleteTextures(1, &image);
    }
    GLuint image { 0 };
    Info info;
};
static Handle s_CurrentHandle = 0;
static std::map<Handle, Impl> s_Images;
Handle Create(const Device::Handle& a_Device, const Info& a_Info)
{
    Impl impl;
    impl.info = a_Info;
    glGenTextures(1, &impl.image);
    auto internalformat { GL::GetFormat(a_Info.format) };
    if (a_Info.samples == Info::Samples::Samples1) {
        switch (a_Info.type) {
        case Image::Info::Type::Image1D:
            glTextureStorage1D(
                impl.image,
                a_Info.mipLevels,
                internalformat,
                a_Info.extent.width);
            break;
        case Image::Info::Type::Image2D:
            glTextureStorage2D(
                impl.image,
                a_Info.mipLevels,
                internalformat,
                a_Info.extent.width,
                a_Info.extent.height);
            break;
        case Image::Info::Type::Image3D:
            glTextureStorage3D(
                impl.image,
                a_Info.mipLevels,
                internalformat,
                a_Info.extent.width,
                a_Info.extent.height,
                a_Info.extent.depth);
            break;
        default:
            break;
        }
    } else {
        switch (a_Info.type) {
        case Image::Info::Type::Image1D:
            throw std::runtime_error("Cannot create multisampled 1D textures");
            break;
        case Image::Info::Type::Image2D:
            glTextureStorage2DMultisample(
                impl.image,
                Uint32(a_Info.samples),
                internalformat,
                a_Info.extent.width,
                a_Info.extent.height,
                a_Info.fixedSampleLocations);
            break;
        case Image::Info::Type::Image3D:
            glTextureStorage3DMultisample(
                impl.image,
                Uint32(a_Info.samples),
                a_Info.mipLevels,
                internalformat,
                a_Info.extent.width,
                a_Info.extent.height,
                a_Info.fixedSampleLocations);
            break;
        default:
            break;
        }
    }
    ++s_CurrentHandle;
    s_Images[s_CurrentHandle] = impl;
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
}
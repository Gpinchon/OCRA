/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:31:47
*/
#include <GL/Image/Format.hpp>
#include <Handle.hpp>
#include <Image/Image.hpp>

#include <map>
#include <stdexcept>

#include <GL/glew.h>

namespace OCRA::Image {
struct Impl {
	Impl(const Device::Handle& a_Device, const Info& a_Info)
	: info(a_Info)
	{
		glGenTextures(1, &handle);
		auto internalformat { GetGLFormat(a_Info.format) };
		if (a_Info.samples == Info::Samples::Samples1) {
			switch (a_Info.type) {
			case Image::Info::Type::Image1D:
				glTextureStorage1D(
					handle,
					a_Info.mipLevels,
					internalformat,
					a_Info.extent.width);
				break;
			case Image::Info::Type::Image2D:
				glTextureStorage2D(
					handle,
					a_Info.mipLevels,
					internalformat,
					a_Info.extent.width,
					a_Info.extent.height);
				break;
			case Image::Info::Type::Image3D:
				glTextureStorage3D(
					handle,
					a_Info.mipLevels,
					internalformat,
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
					internalformat,
					a_Info.extent.width,
					a_Info.extent.height,
					a_Info.fixedSampleLocations);
				break;
			case Image::Info::Type::Image3D:
				glTextureStorage3DMultisample(
					handle,
					Uint32(a_Info.samples),
					a_Info.mipLevels,
					internalformat,
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
}
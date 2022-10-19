#include <GL/Image/Sampler.hpp>
#include <GL/Common/Compare.hpp>
#include <GL/Device.hpp>

#include <GL/glew.h>

namespace OCRA::Image::Sampler {
static inline auto GetGLAddressMode(const AddressMode& a_AddressMode) {
	switch (a_AddressMode)
	{
	case OCRA::Image::Sampler::AddressMode::Repeat:
		return GL_REPEAT;
	case OCRA::Image::Sampler::AddressMode::MirroredRepeat:
		return GL_MIRRORED_REPEAT;
	case OCRA::Image::Sampler::AddressMode::ClampToEdge:
		return GL_CLAMP_TO_EDGE;
	case OCRA::Image::Sampler::AddressMode::ClampToBorder:
		return GL_CLAMP_TO_BORDER;
	default:
		throw std::runtime_error("Unknown Address Mode");
	}
}

static inline auto GetGLMagFilter(const Filter& a_Filter) {
	switch (a_Filter)
	{
	case OCRA::Image::Filter::Nearest:
		return GL_NEAREST;
	case OCRA::Image::Filter::Linear:
		return GL_LINEAR;
	default:
		throw std::runtime_error("Unknown Filter Mode");
	}
}

static inline auto GetGLMinFilter(const Filter& a_Filter, const Filter& a_MipmapMode) {
	switch (a_Filter)
	{
	case OCRA::Image::Filter::Nearest:
		return (a_MipmapMode == Filter::Nearest) ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST_MIPMAP_LINEAR;
	case OCRA::Image::Filter::Linear:
		return (a_MipmapMode == Filter::Nearest) ? GL_LINEAR_MIPMAP_NEAREST : GL_LINEAR_MIPMAP_LINEAR;;
	default:
		throw std::runtime_error("Unknown Filter Mode");
	}
}

static inline auto CreateImageSampler(const Device::Handle& a_Device, const Info& a_Info)
{
	uint32_t handle{ 0 };
	a_Device->PushCommand([&handle, &a_Info] {
		glGenSamplers(1, &handle);
		glSamplerParameteri(handle, GL_TEXTURE_MIN_FILTER, GetGLMinFilter(a_Info.minFilter, a_Info.mipmapMode));
		glSamplerParameteri(handle, GL_TEXTURE_MAG_FILTER, GetGLMagFilter(a_Info.magFilter));
		glSamplerParameteri(handle, GL_TEXTURE_WRAP_S, GetGLAddressMode(a_Info.addressModeU));
		glSamplerParameteri(handle, GL_TEXTURE_WRAP_T, GetGLAddressMode(a_Info.addressModeV));
		glSamplerParameteri(handle, GL_TEXTURE_WRAP_R, GetGLAddressMode(a_Info.addressModeW));
		glSamplerParameterf(handle, GL_TEXTURE_LOD_BIAS, a_Info.mipLodBias);
		if (a_Info.anisotropyEnable) glSamplerParameterf(handle, GL_TEXTURE_MAX_ANISOTROPY, a_Info.maxAnisotropy);
		glSamplerParameteri(handle, GL_TEXTURE_COMPARE_MODE, a_Info.compareEnable ? GL_COMPARE_REF_TO_TEXTURE : GL_NONE);
		if (a_Info.compareEnable) glSamplerParameteri(handle, GL_TEXTURE_COMPARE_FUNC, GetGLOperation(a_Info.compareOp));
		glSamplerParameterf(handle, GL_TEXTURE_MIN_LOD, a_Info.minLod);
		glSamplerParameterf(handle, GL_TEXTURE_MAX_LOD, a_Info.maxLod);
		glSamplerParameterfv(handle, GL_TEXTURE_BORDER_COLOR, &a_Info.borderColor.R);
	}, true);
	return handle;
}
Impl::Impl(const Device::Handle& a_Device, const Info& a_Info)
	: device(a_Device)
	, handle(CreateImageSampler(a_Device, a_Info))
	, info(a_Info)
{
}
Handle Create(const Device::Handle& a_Device, const Info& a_Info) {
	return Handle(new Impl(a_Device, a_Info));
}

}

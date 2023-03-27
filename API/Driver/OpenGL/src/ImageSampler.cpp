#include <GL/Device.hpp>
#include <GL/Enums.hpp>
#include <GL/ImageSampler.hpp>

#include <GL/glew.h>

namespace OCRA::Device {
Image::Sampler::Handle CreateImageSampler(const Device::Handle& a_Device, const CreateImageSamplerInfo& a_Info) {
    return std::make_shared<Image::Sampler::Impl>(a_Device, a_Info);
}
}

namespace OCRA::Image::Sampler {
static inline auto CreateGLImageSampler(const Device::Handle& a_Device, const CreateImageSamplerInfo& a_Info)
{
    uint32_t handle{ 0 };
    
    return handle;
}
Impl::Impl(const Device::Handle& a_Device, const CreateImageSamplerInfo& a_Info)
    : device(a_Device)
    , handle(CreateGLImageSampler(a_Device, a_Info))
    , info(a_Info)
{
    a_Device->PushCommand([this] {
        glGenSamplers(1, &handle);
        glSamplerParameteri(handle, GL_TEXTURE_MIN_FILTER, GetGLMinFilter(info.minFilter, info.mipmapMode));
        glSamplerParameteri(handle, GL_TEXTURE_MAG_FILTER, GetGLMagFilter(info.magFilter));
        glSamplerParameteri(handle, GL_TEXTURE_WRAP_S, GetGLAddressMode(info.addressModeU));
        glSamplerParameteri(handle, GL_TEXTURE_WRAP_T, GetGLAddressMode(info.addressModeV));
        glSamplerParameteri(handle, GL_TEXTURE_WRAP_R, GetGLAddressMode(info.addressModeW));
        glSamplerParameterf(handle, GL_TEXTURE_LOD_BIAS, info.mipLodBias);
        if (info.anisotropyEnable) glSamplerParameterf(handle, GL_TEXTURE_MAX_ANISOTROPY, info.maxAnisotropy);
        glSamplerParameteri(handle, GL_TEXTURE_COMPARE_MODE, info.compareEnable ? GL_COMPARE_REF_TO_TEXTURE : GL_NONE);
        if (info.compareEnable) glSamplerParameteri(handle, GL_TEXTURE_COMPARE_FUNC, GetGLOperation(info.compareOp));
        glSamplerParameterf(handle, GL_TEXTURE_MIN_LOD, info.minLod);
        glSamplerParameterf(handle, GL_TEXTURE_MAX_LOD, info.maxLod);
        glSamplerParameterfv(handle, GL_TEXTURE_BORDER_COLOR, info.borderColor.value);
    }, false);
}
Impl::~Impl()
{
    device.lock()->PushCommand([handle=handle] {
        glDeleteSamplers(1, &handle);
    }, false);
}
}

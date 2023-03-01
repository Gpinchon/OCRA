#include <GL/Device.hpp>
#include <GL/Enums.hpp>
#include <GL/Image/Sampler.hpp>

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
Impl::Impl(const Device::Handle& a_Device, const CreateImageSamplerInfo& a_Info)
    : device(a_Device)
    , handle(CreateGLImageSampler(a_Device, a_Info))
    , info(a_Info)
{}
Impl::~Impl()
{
    device.lock()->PushCommand([handle=handle] {
        glDeleteSamplers(1, &handle);
    }, false);
}
}

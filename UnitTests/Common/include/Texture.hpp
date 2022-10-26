#pragma once

#include <Image/Image.hpp>
#include <Image/Sampler.hpp>
#include <Image/View.hpp>
#include <Descriptor/Pool.hpp>
#include <Descriptor/Set.hpp>
#include <Descriptor/SetLayout.hpp>

OCRA_DECLARE_HANDLE(OCRA::Descriptor::Pool);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::Set);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::SetLayout);

namespace OCRA {
class Texture {
public:
    Texture() = default;
    Texture(const Device::Handle& a_Device, Image::Type a_Type, Image::Format a_Format, const unsigned& a_X, const unsigned& a_Y, const unsigned& a_Z, const unsigned& a_MipLevels)
        : device(a_Device)
        , sampler(Image::Sampler::Create(device, samplerInfo))
    {
        imageInfo.type = a_Type;
        imageInfo.format = a_Format;
        imageInfo.extent = { a_X, a_Y, a_Z };
        imageInfo.mipLevels = a_MipLevels;
        image = Image::Create(device, imageInfo);
    }
    auto& GetSampler() const { return sampler; }
    auto& GetSamplerInfo() const { return samplerInfo; }
    void SetSamplerInfo(const Image::Sampler::Info& a_Info) {
        samplerInfo = a_Info;
        sampler = Image::Sampler::Create(device, samplerInfo);
    };
    auto& GetImageView() const { return imageView; }
    auto& GetImageViewInfo() const { return imageViewInfo; }
    void SetImageViewInfo(const Image::View::Info& a_Info) {
        imageViewInfo = a_Info;
        imageView = Image::View::Create(device, imageViewInfo);
    }
    auto& GetImage() const { return image; }
    auto& GetImageInfo() const { return imageInfo; }
    
private:
    Device::Handle                device;
    Image::Info                   imageInfo{};
    Image::Handle                 image;
    Image::View::Info             imageViewInfo{};
    Image::View::Handle           imageView;
    Image::Sampler::Info          samplerInfo{};
    Image::Sampler::Handle        sampler;
};

class Texture1D : public Texture {
public:
    Texture1D() = default;
    Texture1D(const Device::Handle& a_Device, Image::Format a_Format, const unsigned& a_X, const unsigned a_MipLevels = 1)
        : Texture(a_Device, Image::Type::Image1D, a_Format, a_X, 1, 1, a_MipLevels)
    {
        Image::View::Info imageViewInfo;
        imageViewInfo.image = GetImage();
        imageViewInfo.type = Image::View::Type::View1D;
        imageViewInfo.format = a_Format;
        SetImageViewInfo(imageViewInfo);
    }
};

class Texture2D : public Texture {
public:
    Texture2D() = default;
    Texture2D(const Device::Handle& a_Device, Image::Format a_Format, const unsigned& a_X, const unsigned& a_Y, const unsigned a_MipLevels = 1)
        : Texture(a_Device, Image::Type::Image2D, a_Format, a_X, a_Y, 1, a_MipLevels)
    {
        Image::View::Info imageViewInfo;
        imageViewInfo.image = GetImage();
        imageViewInfo.type = Image::View::Type::View2D;
        imageViewInfo.format = a_Format;
        SetImageViewInfo(imageViewInfo);
    }
    auto GetWidth() const { return GetImageInfo().extent.width; };
    auto GetHeight() const { return GetImageInfo().extent.height; };
private:
};

class Texture3D : public Texture {
public:
    Texture3D() = default;
    Texture3D(const Device::Handle& a_Device, Image::Format a_Format, const unsigned& a_X, const unsigned& a_Y, const unsigned& a_Z, const unsigned a_MipLevels = 1)
        : Texture(a_Device, Image::Type::Image3D, a_Format, a_X, a_Y, a_Z, a_MipLevels)
    {
        Image::View::Info imageViewInfo;
        imageViewInfo.image = GetImage();
        imageViewInfo.type = Image::View::Type::View3D;
        imageViewInfo.format = a_Format;
        SetImageViewInfo(imageViewInfo);
    }
};
}

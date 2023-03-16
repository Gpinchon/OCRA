#pragma once

#include <OCRA/OCRA.hpp>

namespace OCRA {
class Texture {
public:
    Texture() = default;
    Texture(const Device::Handle& a_Device, ImageType a_Type, Format a_Format, const unsigned& a_X, const unsigned& a_Y, const unsigned& a_Z, const unsigned& a_MipLevels)
        : device(a_Device)
        , sampler(CreateImageSampler(device, samplerInfo))
    {
        imageInfo.usage = ImageUsageFlagBits::Sampled;
        imageInfo.type = a_Type;
        imageInfo.format = a_Format;
        imageInfo.extent = { a_X, a_Y, a_Z };
        imageInfo.mipLevels = a_MipLevels;
        imageInfo.arrayLayers = 1;
        image = CreateImage(device, imageInfo);
    }
    auto& GetSampler() const { return sampler; }
    auto& GetSamplerInfo() const { return samplerInfo; }
    void SetSamplerInfo(const CreateImageSamplerInfo& a_Info) {
        samplerInfo = a_Info;
        sampler = CreateImageSampler(device, samplerInfo);
    };
    auto& GetImageView() const { return imageView; }
    auto& GetImageViewInfo() const { return imageViewInfo; }
    void SetImageViewInfo(const CreateImageViewInfo& a_Info) {
        imageViewInfo = a_Info;
        imageView = CreateImageView(device, imageViewInfo);
    }
    auto& GetImage() const { return image; }
    auto& GetImageInfo() const { return imageInfo; }
    
private:
    Device::Handle                device;
    CreateImageInfo               imageInfo{};
    Image::Handle                 image;
    CreateImageViewInfo           imageViewInfo{};
    Image::View::Handle           imageView;
    CreateImageSamplerInfo        samplerInfo{};
    Image::Sampler::Handle        sampler;
};

class Texture1D : public Texture {
public:
    Texture1D() = default;
    Texture1D(const Device::Handle& a_Device, Format a_Format, const unsigned& a_X, const unsigned a_MipLevels = 1)
        : Texture(a_Device, ImageType::Image1D, a_Format, a_X, 1, 1, a_MipLevels)
    {
        CreateImageViewInfo imageViewInfo;
        imageViewInfo.image = GetImage();
        imageViewInfo.type = ImageViewType::View1D;
        imageViewInfo.format = a_Format;
        SetImageViewInfo(imageViewInfo);
    }
};

class Texture2D : public Texture {
public:
    Texture2D() = default;
    Texture2D(const Device::Handle& a_Device, Format a_Format, const unsigned& a_X, const unsigned& a_Y, const unsigned a_MipLevels = 1)
        : Texture(a_Device, ImageType::Image2D, a_Format, a_X, a_Y, 1, a_MipLevels)
    {
        CreateImageViewInfo imageViewInfo;
        imageViewInfo.image = GetImage();
        imageViewInfo.type = ImageViewType::View2D;
        imageViewInfo.format = a_Format;
        imageViewInfo.subRange.aspects = ImageAspectFlagBits::Color;
        imageViewInfo.subRange.levelCount = 1;
        SetImageViewInfo(imageViewInfo);
    }
    auto GetWidth() const { return GetImageInfo().extent.width; };
    auto GetHeight() const { return GetImageInfo().extent.height; };
private:
};

class Texture3D : public Texture {
public:
    Texture3D() = default;
    Texture3D(const Device::Handle& a_Device, Format a_Format, const unsigned& a_X, const unsigned& a_Y, const unsigned& a_Z, const unsigned a_MipLevels = 1)
        : Texture(a_Device, ImageType::Image3D, a_Format, a_X, a_Y, a_Z, a_MipLevels)
    {
        CreateImageViewInfo imageViewInfo;
        imageViewInfo.image = GetImage();
        imageViewInfo.type = ImageViewType::View3D;
        imageViewInfo.format = a_Format;
        SetImageViewInfo(imageViewInfo);
    }
};
}

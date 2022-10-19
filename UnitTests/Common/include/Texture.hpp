#pragma once

#include <Image/Image.hpp>
#include <Image/Sampler.hpp>
#include <Image/View.hpp>
#include <Descriptor/Pool.hpp>
#include <Descriptor/SetLayout.hpp>

OCRA_DECLARE_HANDLE(OCRA::Descriptor::Pool);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::Set);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::SetLayout);

template<uint32_t Binding>
class Texture {
public:
    Texture(const OCRA::Device::Handle& a_Device, OCRA::Image::Type a_Type, OCRA::Image::Format a_Format, const unsigned& a_X, const unsigned& a_Y, const unsigned& a_Z, const unsigned& a_MipLevels)
        : device(a_Device)
        , sampler(OCRA::Image::Sampler::Create(device, samplerInfo))
    {
        imageInfo.type = a_Type;
        imageInfo.format = a_Format;
        imageInfo.extent = { a_X, a_Y, a_Z };
        imageInfo.mipLevels = a_MipLevels;
        image = OCRA::Image::Create(device, imageInfo);
        {
            OCRA::Descriptor::Pool::Info poolInfo{};
            poolInfo.maxSets = 4096;
            poolInfo.sizes = {};
            descriptorPool = OCRA::Descriptor::Pool::Create(device, poolInfo);
        }
        {
            OCRA::Descriptor::SetLayout::Binding binding;
            binding.binding = Binding;
            binding.count = 1;
            binding.type = OCRA::Descriptor::Type::SampledImage;
            OCRA::Descriptor::SetLayout::Info info;
            info.bindings.push_back(binding);
            descriptorSetLayout = OCRA::Descriptor::SetLayout::Create(device, info);
        }
        {
            OCRA::Descriptor::Pool::AllocateInfo allocateInfo{};
            allocateInfo.layouts.push_back(descriptorSetLayout);
            allocateInfo.pool = descriptorPool;
            descriptorSet = OCRA::Descriptor::Pool::AllocateSet(device, allocateInfo).front();
        }
        UpdateDescriptorSet();
    }
    void UpdateDescriptorSet() {
        OCRA::Descriptor::Set::ImageInfo setImageInfo;
        OCRA::Descriptor::Set::WriteOperation writeOp;
        setImageInfo.imageView = imageView;
        setImageInfo.sampler = sampler;
        writeOp.type = OCRA::Descriptor::Type::SampledImage;
        writeOp.dstSet = GetDescriptorSet();
        writeOp.imageInfo = setImageInfo;
        OCRA::Descriptor::Set::Update(device, { writeOp }, {});
    }
    auto& GetSampler() const { return sampler; }
    auto& GetSamplerInfo() const { return samplerInfo; }
    void SetSamplerInfo(const OCRA::Image::Sampler::Info& a_Info) {
        samplerInfo = a_Info;
        sampler = OCRA::Image::Sampler::Create(device, samplerInfo);
        UpdateDescriptorSet();
    };
    auto& GetImageView() const { return imageView; }
    auto& GetImageViewInfo() const { return imageViewInfo; }
    void SetImageViewInfo(const OCRA::Image::View::Info& a_Info) {
        imageViewInfo = a_Info;
        imageView = OCRA::Image::View::Create(device, imageViewInfo);
        UpdateDescriptorSet();
    }
    auto& GetImage() const { return image; }
    auto& GetImageInfo() const { return imageInfo; }
    auto& GetDescriptorSet() const { return descriptorSet; }
    auto& GetDescriptorSetLayout() const { return descriptorSetLayout; }
private:
    OCRA::Device::Handle                device;
    OCRA::Image::Info                   imageInfo{};
    OCRA::Image::Handle                 image;
    OCRA::Image::View::Info             imageViewInfo{};
    OCRA::Image::View::Handle           imageView;
    OCRA::Image::Sampler::Info          samplerInfo{};
    OCRA::Image::Sampler::Handle        sampler;
    OCRA::Descriptor::Pool::Handle      descriptorPool;
    OCRA::Descriptor::Set::Handle       descriptorSet;
    OCRA::Descriptor::SetLayout::Handle descriptorSetLayout;
};

template<uint32_t Binding>
class Texture1D : public Texture<Binding> {
public:
    Texture1D(const OCRA::Device::Handle& a_Device, OCRA::Image::Format a_Format, const unsigned& a_X, const unsigned a_MipLevels = 1)
        : Texture(a_Device, OCRA::Image::Type::Image1D, a_Format, a_X, 1, 1, a_MipLevels)
    {
        OCRA::Image::View::Info imageViewInfo;
        imageViewInfo.image = GetImage();
        imageViewInfo.type = OCRA::Image::View::Type::View1D;
        imageViewInfo.format = a_Format;
        SetImageViewInfo(imageViewInfo);
    }
};

template<uint32_t Binding>
class Texture2D : public Texture<Binding> {
public:
    Texture2D(const OCRA::Device::Handle& a_Device, OCRA::Image::Format a_Format, const unsigned& a_X, const unsigned& a_Y, const unsigned a_MipLevels = 1)
        : Texture(a_Device, OCRA::Image::Type::Image2D, a_Format, a_X, a_Y, 1, a_MipLevels)
        , width(a_X)
        , height(a_Y)
    {
        OCRA::Image::View::Info imageViewInfo;
        imageViewInfo.image = GetImage();
        imageViewInfo.type = OCRA::Image::View::Type::View2D;
        imageViewInfo.format = a_Format;
        SetImageViewInfo(imageViewInfo);
    }
    auto GetWidth() const { return width; };
    auto GetHeight() const { return height; };
private:
    unsigned width{ 0 };
    unsigned height{ 0 };
};

template<uint32_t Binding>
class Texture3D : public Texture<Binding> {
public:
    Texture3D(const OCRA::Device::Handle& a_Device, OCRA::Image::Format a_Format, const unsigned& a_X, const unsigned& a_Y, const unsigned& a_Z, const unsigned a_MipLevels = 1)
        : Texture(a_Device, OCRA::Image::Type::Image3D, a_Format, a_X, a_Y, a_Z, a_MipLevels)
    {
        OCRA::Image::View::Info imageViewInfo;
        imageViewInfo.image = GetImage();
        imageViewInfo.type = OCRA::Image::View::Type::View3D;
        imageViewInfo.format = a_Format;
        SetImageViewInfo(imageViewInfo);
    }
};
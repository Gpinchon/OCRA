#pragma once

#include <Image/Image.hpp>

class Texture {
public:
    Texture(const OCRA::Device::Handle& a_Device, OCRA::Image::Type a_Type, OCRA::Image::Format a_Format, const unsigned& a_X, const unsigned& a_Y, const unsigned& a_Z, const unsigned& a_MipLevels)
        : format(a_Format)
    {
        OCRA::Image::Info imageInfo;
        imageInfo.type = a_Type;
        imageInfo.format = a_Format;
        imageInfo.extent = { a_X, a_Y, a_Z };
        imageInfo.mipLevels = a_MipLevels;
        image = OCRA::Image::Create(a_Device, imageInfo);
    }
    auto& GetImage() const { return image; };
    auto GetFormat() const { return format; }
private:
    OCRA::Image::Handle image;
    OCRA::Image::Format format;
};

class Texture1D : public Texture {
public:
    Texture1D(const OCRA::Device::Handle& a_Device, OCRA::Image::Format a_Format, const unsigned& a_X, const unsigned a_MipLevels = 1)
        : Texture(a_Device, OCRA::Image::Type::Image1D, a_Format, a_X, 1, 1, a_MipLevels)
    {}
};

class Texture2D : public Texture {
public:
    Texture2D(const OCRA::Device::Handle& a_Device, OCRA::Image::Format a_Format, const unsigned& a_X, const unsigned& a_Y, const unsigned a_MipLevels = 1)
        : Texture(a_Device, OCRA::Image::Type::Image2D, a_Format, a_X, a_Y, 1, a_MipLevels)
        , width(a_X)
        , height(a_Y)
    {}
    auto GetWidth() const { return width; };
    auto GetHeight() const { return height; };
private:
    unsigned width{ 0 };
    unsigned height{ 0 };
};

class Texture3D : public Texture {
public:
    Texture3D(const OCRA::Device::Handle& a_Device, OCRA::Image::Format a_Format, const unsigned& a_X, const unsigned& a_Y, const unsigned& a_Z, const unsigned a_MipLevels = 1)
        : Texture(a_Device, OCRA::Image::Type::Image3D, a_Format, a_X, a_Y, a_Z, a_MipLevels)
    {}
};
/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:28:24
*/
#pragma once

#include <Common/Compare.hpp>
#include <Image/Filter.hpp>

OCRA_DECLARE_HANDLE(OCRA::Image::Sampler);

namespace OCRA::Image::Sampler {
enum class MipMapMode {
    Nearest,
    Linear
};
enum class AddressMode {
    Repeat,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder,
    MaxValue
};
struct BorderColor {
    float R { 0 }, G { 0 }, B { 0 }, A { 0 };
};
constexpr BorderColor TransparentBlack = BorderColor { 0, 0, 0, 0 };
constexpr BorderColor OpaqueBlack = BorderColor { 0, 0, 0, 1 };
constexpr BorderColor OpaqueWhite = BorderColor { 1, 1, 1, 1 };

struct Info {
    Filter minFilter { Filter::Linear };
    Filter magFilter { Filter::Linear };
    MipMapMode mipmapMode { MipMapMode::Linear };
    AddressMode addressModeU { AddressMode::Repeat };
    AddressMode addressModeV { AddressMode::Repeat };
    AddressMode addressModeW { AddressMode::Repeat };
    float mipLodBias { 0 };
    bool anisotropyEnable { false };
    float maxAnisotropy { 0 };
    bool compareEnable { false };
    Compare::Operation compareOp { Compare::Operation::Unknown };
    float minLod { 0 };
    float maxLod { 1000 };
    BorderColor borderColor { TransparentBlack };
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info);
}
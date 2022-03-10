/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-06 19:39:36
*/
#pragma once

#include <Device.hpp>
#include <Extent3D.hpp>
#include <Handle.hpp>
#include <Image/Format.hpp>

#include <bitset>
#include <vector>

HANDLE(OCRA::Image);

namespace OCRA::Image {
//Image Info specs
struct Info {
    enum class Type {
        Unknown = -1,
        Image1D,
        Image2D,
        Image3D,
        MaxValue
    } type { Type::Unknown };
    enum class Samples {
        Samples1 = 1,
        Samples2 = 2,
        Samples4 = 4,
        Samples8 = 8,
        Samples16 = 16,
        Samples32 = 32,
        Samples64 = 64,
        MaxValue
    } samples { Samples::Samples1 };
    Image::Format format { Image::Format::Unknown };
    Extent3D extent;
    uint16_t mipLevels { 0 }, arrayLayers { 0 };
    bool fixedSampleLocations { false };
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info);
}
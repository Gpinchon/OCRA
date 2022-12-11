/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:28:28
*/
#pragma once

#include <OCRA/Handle.hpp>
#include <OCRA/Image/Format.hpp>
#include <OCRA/Image/Image.hpp>
#include <OCRA/Common/Component.hpp>

OCRA_DECLARE_HANDLE(OCRA::Image::View);

namespace OCRA::Image::View {
struct SubRange {
    uint16_t baseMipLevel { 0 }, levelCount { 1000 };
    uint16_t baseArrayLayer { 0 }, layerCount { 1 };
};
enum class Type {
    Unknown = -1,
    View1D,
    View2D,
    View3D,
    ViewCube,
    View1DArray,
    View2DArray,
    ViewCubeArray,
    MaxValue
};
//Image View specs
struct Info {
    Type type { Type::Unknown };
    Image::Handle image { 0 };
    Image::Format format { Image::Format::Unknown };
    Component::Mapping components {};
    SubRange subRange {};
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info);
}
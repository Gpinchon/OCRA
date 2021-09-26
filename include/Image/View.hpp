/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:28:28
*/
#pragma once

#include <Handle.hpp>
#include <Image/Format.hpp>
#include <Scalar.hpp>

namespace OCRA::Image::View {
struct SubRange {
    Uint16 baseMipLevel { 0 }, levelCount { 1000 };
    Uint16 baseArrayLayer { 0 }, layerCount { 1000 };
};
enum class Type {
    Unknown = -1,
    View1D,
    View2D,
    view3D,
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
    SubRange subRange {};
};
}
#pragma once

namespace OCRA::Image::View
{
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
}
#pragma once

#include <Common/Vec3.hpp>
#include <Common/Extent3D.hpp>

namespace OCRA {
struct Rect3D
{
    iVec3     offset;
    uExtent3D extent;
};
}
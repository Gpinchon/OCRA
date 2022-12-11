#pragma once

#include <OCRA/Common/Vec3.hpp>
#include <OCRA/Common/Extent3D.hpp>

namespace OCRA {
struct Rect3D
{
    iVec3     offset;
    uExtent3D extent;
};
}
#pragma once

#include <Common/Vec2.hpp>
#include <Common/Extent2D.hpp>

namespace OCRA {
struct Rect2D
{
    iVec2     offset{ 0, 0 };
    uExtent2D extent{ 0, 0 };
};
}
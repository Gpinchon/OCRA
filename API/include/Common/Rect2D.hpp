#pragma once

#include <Common/Vec2.hpp>
#include <Common/Extent2D.hpp>

namespace OCRA {
struct Rect2D
{
	Rect2D(int a_X = 0, int a_Y = 0, unsigned a_Width = 0, unsigned a_Height = 0)
		: offset(a_X, a_Y), extent(a_Width, a_Height)
	{}
    iVec2     offset{ 0, 0 };
    uExtent2D extent{ 0, 0 };
};
}
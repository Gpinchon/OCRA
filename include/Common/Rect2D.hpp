#pragma once

#include <Common/Rect.hpp>
#include <Common/Vec2.hpp>
#include <Common/Extent2D.hpp>

namespace OCRA {
template<typename T>
struct Rect<2, T> {
	typedef T value_type;
	typedef Rect<2, T> type;
	constexpr auto length() { return 2; }
	Vec<2, T> offset;
	Extent<2, T> extent;
};
typedef Rect<2, float> Rect2D;
}
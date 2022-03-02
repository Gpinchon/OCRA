#pragma once

#include <Common/Rect.hpp>
#include <Common/Vec3.hpp>
#include <Common/Extent3D.hpp>

namespace OCRA {
template<typename T>
struct Rect<3, T> {
	typedef T value_type;
	typedef Rect<3, T> type;
	constexpr auto length() { return 3; }
	Vec<3, T> offset;
	Extent<3, T> extent;
};
typedef Rect<3, float> Rect3D;
}
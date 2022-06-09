#pragma once

#include <Common/Extent.hpp>

namespace OCRA {
template <typename T>
struct Extent<3, T> {
	typedef T value_type;
	typedef Extent<3, T> type;
	constexpr auto length() { return 3; }
	Extent<3, T>(const T& a_Width = T(0), const T& a_Height = T(0), const T& a_Depth = T(0)) : width(a_Width), height(a_Height), depth(a_Depth) {}
	struct { T width, height, depth; };
};
typedef Extent<3, float> Extent3D;
}
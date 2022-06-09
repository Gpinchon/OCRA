#pragma once

#include <Common/Extent.hpp>

namespace OCRA {
template <typename T>
struct Extent<2, T> {
	typedef T value_type;
	typedef Extent<2, T> type;
	constexpr auto length() { return 2; }
	Extent<2, T>(const T& a_Width = T(0), const T& a_Height = T(0)) : width(a_Width), height(a_Height) {}
	struct { T width, height; };
};
typedef Extent<2, float>    Extent2D;
typedef Extent<2, int>		iExtent2D;
typedef Extent<2, unsigned> uExtent2D;
}
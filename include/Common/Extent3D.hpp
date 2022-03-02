#pragma once

#include <Common/Extent.hpp>

namespace OCRA {
template <typename T>
struct Extent<3, T> {
	typedef T value_type;
	typedef Extent<3, T> type;
	constexpr auto length() { return 3; }
	struct { T width, height, depth; };
};
typedef Extent<3, float> Extent3D;
}
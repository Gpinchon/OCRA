#pragma once

#include <Common/Extent.hpp>

namespace OCRA {
template <typename T>
struct Extent<2, T> {
	typedef T value_type;
	typedef Extent<2, T> type;
	constexpr auto length() { return 2; }
	struct { T width, height; };
};
typedef Extent<2, float> Extent2D;
}
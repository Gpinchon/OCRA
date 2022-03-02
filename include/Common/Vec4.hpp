#pragma once

#include <Common/Vec.hpp>

namespace OCRA {
template <typename T>
struct Vec<4, T> {
	typedef T value_type;
	typedef Vec<4, T> type;
	constexpr auto length() const { return 4; }
	union {
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };
		struct { T u, v, s, t; };
	};
};
typedef Vec<4, float> Vec4;
}
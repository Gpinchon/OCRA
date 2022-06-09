#pragma once

#include <Common/Vec.hpp>

namespace OCRA {
template <typename T>
struct Vec<3, T> {
	typedef T value_type;
	typedef Vec<3, T> type;
	constexpr auto length() const { return 3; }
	union {
		struct { T x, y, z; };
		struct { T r, g, b; };
		struct { T u, v, s; };
	};
};
typedef Vec<3, float> Vec3;
}
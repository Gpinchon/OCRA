#pragma once

#include <Common/Vec.hpp>

namespace OCRA {
template <typename T>
struct Vec<2, T> {
	typedef T value_type;
	typedef Vec<2, T> type;
	constexpr auto length() const { return 2; }
	union {
		struct { T x, y; };
		struct { T r, g; };
		struct { T u, v; };
	};
};
typedef Vec<2, float>	 Vec2;
typedef Vec<2, double>	 dVec2;
typedef Vec<2, int>		 iVec2;
typedef Vec<2, unsigned> uVec2;
typedef Vec<2, bool>     bVec2;
}
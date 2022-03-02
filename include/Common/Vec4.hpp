#pragma once

#include <Common/Vec.hpp>

namespace OCRA {
template <typename T = float>
struct Vec4 : Vec<4, T> {
	union {
		struct {T x, y, z, w},
		struct {T r, g, b, a},
		struct {T u, v, s, t}
	};
};
}
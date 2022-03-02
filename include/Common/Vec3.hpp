#pragma once

#include <Common/Vec.hpp>

namespace OCRA {
template <typename T = float>
struct Vec3 : Vec<3, T> {
	union {
		struct {T x, y, z},
		struct {T r, g, b},
		struct {T u, v, s}
	};
};
}
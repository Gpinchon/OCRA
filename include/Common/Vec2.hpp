#pragma once

#include <Common/Vec.hpp>

namespace OCRA {
template <typename T = float>
struct Vec2 : Vec<2, T> {
	union {
		struct {T x, y},
		struct {T r, g},
		struct {T u, v}
	};
};
}
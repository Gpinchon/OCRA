#pragma once

#include <Common/Rect.hpp>

namespace OCRA {
template<typename T = float>
struct Rect3D : Rect<3, T> {
	struct {
		T x, y, z;
	} offset;
	struct {
		T width, height, depth;
	} extent;
};
}
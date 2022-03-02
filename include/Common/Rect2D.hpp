#pragma once

#include <Common/Rect.hpp>

namespace OCRA {
template<typename T = float>
struct Rect2D : Rect<2, T> {
	struct {
		T x, y; //left, bottom
	} offset;
	struct {
		T w, h; //width, height
	} extent;
};
}
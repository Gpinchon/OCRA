#pragma once

namespace OCRA {
template <typename T = double>
struct DepthBounds {
	T min{ 0 }, max{ 1000 };
};
}
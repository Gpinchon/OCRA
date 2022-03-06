#pragma once

namespace OCRA {
template <typename T = double>
struct DepthRange {
	T near{ 0 }, far{ 1 };
};
}
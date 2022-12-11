#pragma once

namespace OCRA {
template <typename T = double>
struct DepthRange {
    DepthRange(const T& a_Near, const T& a_Far)
        : near(a_Near), far(a_Far)
    {}
    T near{ 0 }, far{ 1 };
};
}
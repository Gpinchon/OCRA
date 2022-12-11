#pragma once

namespace OCRA {
template <typename T = double>
struct DepthBounds {
    DepthBounds(const T& a_Min = T(0), const T& a_Max = T(0)) {
        min = a_Min;
        max = a_Max;
    }
    T min{ 0 }, max{ 1000 };
};
}
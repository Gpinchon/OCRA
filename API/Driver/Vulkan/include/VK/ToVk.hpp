#pragma once

namespace OCRA {
/**
* invokes the proper ConvertToVk, useful for things like std::transform
*/
struct ToVk {
    template <typename T>
    auto operator()(const T& a_Val)
    {
        return ConvertToVk(a_Val);
    }
};
}
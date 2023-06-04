#pragma once

#include <OCRA/Vec.hpp>
#include <OCRA/Vec_Functor.hpp>

namespace OCRA {
template <unsigned L, typename T>
inline auto equal(const Vec<L, T>& a_X, const Vec<L, T>& a_Y)
{
    return VecFunctor1<L, T, T, bool>::call<std::equal_to<T>>(a_X, a_Y);
}

template <unsigned L, typename T>
inline auto greaterThan(const Vec<L, T>& a_X, const Vec<L, T>& a_Y)
{
    return VecFunctor1<L, T, T, bool>::call<std::greater<T>>(a_X, a_Y);
}
template <unsigned L, typename T>
inline auto greaterThanEqual(const Vec<L, T>& a_X, const Vec<L, T>& a_Y)
{
    return VecFunctor1<L, T, T, bool>::call<std::greater_equal<T>>(a_X, a_Y);
}

template <unsigned L, typename T>
inline auto lessThan(const Vec<L, T>& a_X, const Vec<L, T>& a_Y)
{
    return VecFunctor1<L, T, T, bool>::call<std::less<T>>(a_X, a_Y);
}
template <unsigned L, typename T>
inline auto lessThanEqual(const Vec<L, T>& a_X, const Vec<L, T>& a_Y)
{
    return VecFunctor1<L, T, T, bool>::call<std::less_equal<T>>(a_X, a_Y);
}

template <unsigned L>
inline auto any(const Vec<L, bool>& a_V)
{
    bool result = false;
    for (int i = 0; i < L; ++i)
        result |= a_V[i];
    return result;
}
template <unsigned L>
inline auto all(const Vec<L, bool>& a_V)
{
    bool result = false;
    for (int i = 0; i < L; ++i)
        result &= a_V[i];
    return result;
}

template <unsigned L>
inline auto not(const Vec<L, bool>& a_V)
{
    return VecFunctor<L, bool, bool>::call<std::logical_not<bool>>(a_V);
}
}

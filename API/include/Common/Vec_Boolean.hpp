#pragma once

#include <Common/Vec.hpp>

namespace OCRA {
template<unsigned L, typename T>
inline auto equal(const Vec<L, T>& a_X, const Vec<L, T>& a_Y)
{
    Vec<L, bool> result{};
    for (int i = 0; i < L; ++i)
        result[i] = a_X[i] == a_Y[i];
    return result;
}

template<unsigned L, typename T>
inline auto greaterThan(const Vec<L, T>& a_X, const Vec<L, T>& a_Y)
{
    Vec<L, bool> result{};
    for (int i = 0; i < L; ++i)
        result[i] = a_X[i] > a_Y[i];
    return result;
}
template<unsigned L, typename T>
inline auto greaterThanEqual(const Vec<L, T>& a_X, const Vec<L, T>& a_Y)
{
    Vec<L, bool> result{};
    for (int i = 0; i < L; ++i)
        result[i] = a_X[i] >= a_Y[i];
    return result;
}

template<unsigned L, typename T>
inline auto lessThan(const Vec<L, T>& a_X, const Vec<L, T>& a_Y)
{
    Vec<L, bool> result{};
    for (int i = 0; i < L; ++i)
        result[i] = a_X[i] < a_Y[i];
    return result;
}
template<unsigned L, typename T>
inline auto lessThanEqual(const Vec<L, T>& a_X, const Vec<L, T>& a_Y)
{
    Vec<L, bool> result{};
    for (int i = 0; i < L; ++i)
        result[i] = a_X[i] <= a_Y[i];
    return result;
}

template<unsigned L>
inline auto any(const Vec<L, bool>& a_V)
{
    bool result = false;
    for (int i = 0; i < L; ++i)
        result |= a_V[i];
    return result;
}
template<unsigned L>
inline auto all(const Vec<L, bool>& a_V)
{
    bool result = false;
    for (int i = 0; i < L; ++i)
        result &= a_V[i];
    return result;
}

template<unsigned L>
inline auto not(const Vec<L, bool>& a_V)
{
    Vec<L, bool> result = false;
    for (int i = 0; i < L; ++i)
        result = !a_V[i];
    return result;
}
}
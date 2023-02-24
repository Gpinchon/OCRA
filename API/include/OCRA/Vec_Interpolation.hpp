#pragma once

#include <OCRA/Vec.hpp>
#include <OCRA/Vec_Functor.hpp>

namespace OCRA {
template<typename T>
struct minF {
    [[nodiscard]] constexpr inline T operator()(const T& x, const T& y) const { return (y < x) ? y : x; }
};
template<typename T>
inline auto min(T const& x, T const& y) {
    return minF<T>{}(x, y);
}
template<unsigned L, typename T>
inline auto min(OCRA::Vec<L, T> const& x, OCRA::Vec<L, T> const& y)
{
    return VecFunctor1<L, T, T, T>::call<minF<T>>(x, y);
}
template<unsigned L, typename T>
inline auto min(OCRA::Vec<L, T> const& x, float const& y) {
    return VecFunctor1<L, T, T, T>::call<minF<T>>(x, y);
}

template<typename T>
struct maxF {
    [[nodiscard]] constexpr inline T operator()(const T& x, const T& y) const { return (y > x) ? y : x; }
};
template<typename T>
inline auto max(T const& x, T const& y) {
    return maxF<T>{}(x, y);
}
template<unsigned L, typename T>
inline auto max(OCRA::Vec<L, T> const& x, OCRA::Vec<L, T> const& y) {
    return VecFunctor1<L, T, T, T>::call<maxF<T>>(x, y);
}
template<unsigned L, typename T>
inline auto max(OCRA::Vec<L, T> const& x, float const& y) {
    return VecFunctor1<L, T, T, T>::call<maxF<T>>(x, y);
}

template<typename T>
inline auto clamp(T const& a_X, T const& a_Min, T const& a_Max) {
    return min(max(a_X, a_Min), a_Max);
}
template<unsigned L, typename T>
inline auto clamp(OCRA::Vec<L, T> const& a_X, float const& a_Min, float const& a_Max) {
    return min(max(a_X, a_Min), a_Max);
}

template<typename T>
inline auto saturate(const T& a_Value) {
    return clamp(a_Value, T(0), T(1));
}

template<typename T>
struct stepF {
    [[nodiscard]] constexpr inline T operator()(const T& edge, const T& x) const { return x < edge ? T(0) : T(1); }
};
template<typename T>
inline auto step(T edge, T x) {
    return stepF<T>{}(edge, x);
}
template<unsigned L, typename T>
inline auto step(float const& edge, Vec<L, T> const& x) {
    return VecFunctor1<L, T, T, T>::call<stepF<T>>(edge, x);
}
template<unsigned L, typename T>
inline auto step(Vec<L, T> const& edge, Vec<L, T> const& x) {
    return VecFunctor1<L, T, T, T>::call<stepF<T>>(edge, x);
}

template<typename T>
inline auto mix(T const& x, T const& y, T const& a) {
    return x * (T(1) - a) + y * a;
}
template<typename T>
inline auto mix(T const& x, T const& y, float const& a) {
    return x * (T(1) - a) + y * a;
}

template<typename T>
inline auto smoothStep(T const& edge0, T const& edge1, T const& x) {
    T t = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0 - 2.0 * t);
}
}
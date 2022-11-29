#pragma once

#include <Common/Vec.hpp>
#include <Common/Vec_Functor.hpp>

namespace OCRA
{
template<unsigned L, typename T, typename U>
inline auto operator+(Vec<L, T> const& a_X, Vec<L, U> const& a_Y) {
    return VecFunctor1<L, T, U, T>::call<std::plus<T>>(a_X, a_Y);
}
template<unsigned L, typename T, typename U>
inline auto operator+(Vec<L, T> const& a_X, U const& a_Y) {
    return VecFunctor1<L, T, U, T>::call<std::plus<T>>(a_X, a_Y);
}
template<unsigned L, typename T, typename U>
inline auto operator+(T const& a_X, Vec<L, U> const& a_Y) {
    return VecFunctor1<L, T, U, T>::call<std::plus<T>>(a_X, a_Y);
}

template<unsigned L, typename T, typename U>
inline auto operator-(Vec<L, T> const& a_X, Vec<L, U> const& a_Y) {
    return VecFunctor1<L, T, U, T>::call<std::minus<T>>(a_X, a_Y);
}
template<unsigned L, typename T, typename U>
inline auto operator-(Vec<L, T> const& a_X, U const& a_Y) {
    return VecFunctor1<L, T, U, T>::call<std::minus<T>>(a_X, a_Y);
}
template<unsigned L, typename T, typename U>
inline auto operator-(T const& a_X, Vec<L, U> const& a_Y) {
    return VecFunctor1<L, T, U, T>::call<std::minus<T>>(a_X, a_Y);
}

template<unsigned L, typename T, typename U>
inline auto operator*(Vec<L, T> const& a_X, Vec<L, U> const& a_Y) {
    return VecFunctor1<L, T, U, T>::call<std::multiplies<T>>(a_X, a_Y);
}
template<unsigned L, typename T, typename U>
inline auto operator*(Vec<L, T> const& a_X, U const& a_Y) {
    return VecFunctor1<L, T, U, T>::call<std::multiplies<T>>(a_X, a_Y);
}
template<unsigned L, typename T, typename U>
inline auto operator*(T const& a_X, Vec<L, U> const& a_Y) {
    return VecFunctor1<L, T, U, T>::call<std::multiplies<T>>(a_X, a_Y);
}

template<unsigned L, typename T>
inline auto operator/(Vec<L, T> const& a_X, Vec<L, T> const& a_Y) {
    return VecFunctor1<L, T, T, T>::call<std::divides<T>>(a_X, a_Y);
}
template<unsigned L, typename T, typename U>
inline auto operator/(Vec<L, T> const& a_X, U const& a_Y) {
    return VecFunctor1<L, T, U, T>::call<std::divides<T>>(a_X, a_Y);
}
template<unsigned L, typename T>
inline auto operator/(T const& a_X, Vec<L, T> const& a_Y) {
    return VecFunctor1<L, T, U, T>::call<std::divides<T>>(a_X, a_Y);
}

template<typename T>
struct absF {
    [[nodiscard]] constexpr inline T operator()(const T& x) const { return std::abs(x); }
};
template<unsigned L, typename T>
inline auto abs(const Vec<L, T>& a_X) {
    return VecFunctor<L, T, T>::call<absF<T>>(a_X);
}

template<typename T>
struct floorF {
    [[nodiscard]] constexpr inline T operator()(const T& x) const { return std::floor(x); }
};
template<unsigned L, typename T>
inline auto floor(const Vec<L, T>& a_X) {
    return VecFunctor<L, T, T>::call<floorF<T>>(a_X);
}

template<typename T>
struct ceilF {
    [[nodiscard]] constexpr inline T operator()(const T& x) const { return std::ceil(x); }
};
template<unsigned L, typename T>
inline auto ceil(const Vec<L, T>& a_X) {
    return VecFunctor<L, T, T>::call<ceilF<T>>(a_X);
}

template<typename T>
struct roundF {
    [[nodiscard]] constexpr inline T operator()(const T& x) const { return std::round(x); }
};
template<unsigned L, typename T>
inline auto round(const Vec<L, T>& a_X) {
    return VecFunctor<L, T, T>::call<roundF<T>>(a_X);
}

template<typename T>
struct fractF {
    [[nodiscard]] constexpr inline T operator()(const T& x) const { return x - std::floor(x); }
};
template<typename T>
inline auto fract(const T& a_X) {
    return fractF<T>{}(a_X);
}
template<unsigned L, typename T>
inline auto fract(const Vec<L, T>& a_X) {
    return VecFunctor<L, T, T>::call<fractF<T>>(a_X);
}

template<typename T>
struct signF {
    [[nodiscard]] constexpr inline T operator()(const T& x) const { return (T(0) < x) - (x < T(0)); }
};
template<typename T>
inline auto sign(const T& a_X) {
    return signF<T>{}(a_X);
}
template<unsigned L, typename T>
inline auto sign(const Vec<L, T>& a_X) {
    return VecFunctor<L, T, T>::call<signF<T>>(a_X);
}

template<typename T>
inline auto mod(const T& a_X, const T& a_Y) {
    return std::modulus<T>{}(a_X, a_Y);
}
template<unsigned L, typename T>
inline auto mod(const Vec<L, T>& a_X, const Vec<L, T>& a_Y) {
    return VecFunctor1<L, T, T, T>::call<std::modulus<T>>(a_X, a_Y);
}
template<unsigned L, typename T>
inline auto mod(const Vec<L, T>& a_X, const T& a_Y) {
    return VecFunctor1<L, T, T, T>::call<std::modulus<T>>(a_X, a_Y);
}
template<unsigned L, typename T>
inline auto mod(const T& a_X, const Vec<L, T>& a_Y) {
    return VecFunctor1<L, T, T, T>::call<std::modulus<T>>(a_X, a_Y);
}

template<unsigned L, typename T>
inline auto length(Vec<L, T> const& a_V) {
    T result = T(0);
    for (auto i = 0u; i < L; ++i)
        result += a_V[i] * a_V[i];
    return sqrt(result);
}

template<unsigned L, typename T>
inline auto normalize(Vec<L, T> const& a_V) {
    return a_V * length(a_V);
}
}
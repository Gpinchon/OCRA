#pragma once

#include <Common/Vec.hpp>

namespace OCRA
{
template<unsigned L, typename T, typename U>
inline auto operator+(Vec<L, T> const& a_V0, Vec<L, U> const& a_V1)
{
	Vec<L, T> result;
	for (auto i = 0u; i < L; ++i)
		result[i] = a_V0[i] + a_V1[i];
	return result;
}
template<unsigned L, typename T, typename U>
inline auto operator+(Vec<L, T> const& a_V0, U const& a_X)
{
	Vec<L, T> result;
	for (auto i = 0u; i < L; ++i)
		result[i] = a_V0[i] + a_X;
	return result;
}

template<unsigned L, typename T, typename U>
inline auto operator-(Vec<L, T> const& a_V0, Vec<L, U> const& a_V1)
{
	Vec<L, T> result;
	for (auto i = 0u; i < L; ++i)
		result[i] = a_V0[i] - a_V1[i];
	return result;
}
template<unsigned L, typename T, typename U>
inline auto operator-(Vec<L, T> const& a_V0, U const& a_X)
{
	Vec<L, T> result;
	for (auto i = 0u; i < L; ++i)
		result[i] = a_V0[i] - a_X;
	return result;
}

template<unsigned L, typename T, typename U>
inline auto operator*(Vec<L, T> const& a_V0, Vec<L, U> const& a_V1)
{
	Vec<L, T> result;
	for (auto i = 0u; i < L; ++i)
		result[i] = a_V0[i] * a_V1[i];
	return result;
}
template<unsigned L, typename T, typename U>
inline auto operator*(Vec<L, T> const& a_V, U const& a_X)
{
	Vec<L, T> result;
	for (auto i = 0u; i < L; ++i)
		result[i] = a_V[i] * a_X;
	return result;
}

template<unsigned L, typename T>
inline auto operator/(T const& a_X, Vec<L, T> const& a_V)
{
	Vec<L, T> result;
	for (auto i = 0u; i < L; ++i)
		result[i] = a_X / a_V[i];
	return result;
}
template<unsigned L, typename T>
inline auto operator/(Vec<L, T> const& a_V, Vec<L, T> const& a_V1)
{
	Vec<L, T> result;
	for (auto i = 0u; i < L; ++i)
		result[i] = a_V[i] / a_V1[i];
	return result;
	//return a_V * (T(1) / a_V1);
}
template<unsigned L, typename T, typename U>
inline auto operator/(Vec<L, T> const& a_V, U const& a_X)
{
	Vec<L, T> result;
	for (auto i = 0u; i < L; ++i)
		result[i] = a_V[i] / a_X;
	return result;
	//return a_V * (T(1) / a_X);
}

template<unsigned L, typename T>
inline auto length(Vec<L, T> const& a_V)
{
	T result = T(0);
	for (auto i = 0u; i < L; ++i)
		result += a_V[i] * a_V[i];
	return sqrt(result);
}
}
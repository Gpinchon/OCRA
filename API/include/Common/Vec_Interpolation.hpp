#pragma once

#include <Common/Vec.hpp>

namespace OCRA {
template<typename T>
inline auto step(T edge, T x)
{
	return x < edge ? 0 : 1;
}
template<unsigned L, typename T>
inline auto step(float edge, Vec<L, T> x)
{
	Vec<L, T> result;
	for (int i = 0; i < L; ++i)
		result[i] = x[i] < edge ? 0 : 1;
	return result;
}
template<unsigned L, typename T>
inline auto step(Vec<L, T> edge, Vec<L, T> x)
{
	Vec<L, T> result;
	for (int i = 0; i < L; ++i)
		result[i] = x[i] < edge[i] ? 0 : 1;
	return result;
}

template<typename T>
inline auto mix(T x, T y, T a)
{
	return x * (T(1) - a) + y * a;
}
template<typename T>
inline auto mix(T x, T y, float a)
{
	return x * (T(1) - a) + y * a;
}
}
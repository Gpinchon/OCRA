#pragma once

#include <Common/Vec.hpp>

namespace OCRA {
template <typename T>
struct Vec<3, T> {
	typedef T value_type;
	typedef Vec<3, T> type;
	constexpr auto length() const { return 3; }
	Vec() : value{ 0, 0, 0 } {}
	template<typename X, typename Y, typename Z>
	Vec(const X& a_X = X(0), const Y& a_Y = Y(0), const Z& a_Z = Z(0))
		: x(a_X), y(a_Y), z(a_Z)
	{}
	auto& operator[] (const size_t& a_Index) { return value[a_Index]; }
	auto& operator[] (const size_t& a_Index) const { return value[a_Index]; }
	union {
		struct { value_type x, y, z; };
		struct { value_type r, g, b; };
		struct { value_type u, v, s; };
		value_type value[3];
	};
};

template<typename T>
auto operator*(const T& a_Scalar, Vec<3, T> const& a_V)
{
	return Vec<3, T>(
		a_Scalar * a_V.x,
		a_Scalar * a_V.y,
		a_Scalar * a_V.z);
}

template<typename T>
auto operator*(Vec<3, T> const& a_V, const T& a_Scalar)
{
	return Vec<3, T>(
		a_V.x * a_Scalar,
		a_V.y * a_Scalar,
		a_V.z * a_Scalar);
}

template<typename T>
auto operator/(const T& a_Scalar, Vec<3, T> const& a_V)
{
	return Vec<3, T>(
		a_Scalar / a_V.x,
		a_Scalar / a_V.y,
		a_Scalar / a_V.z);
}

template<typename T>
auto operator/(Vec<3, T> const& a_V, const T& a_Scalar)
{
	return Vec<3, T>(
		a_V.x / a_Scalar,
		a_V.y / a_Scalar,
		a_V.z / a_Scalar);
}

template<typename T>
auto Normalize(Vec<3, T> const& a_V)
{
	auto v = a_V;
	auto len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

	if (len != T(0))
	{
		v.x /= len;
		v.y /= len;
		v.z /= len;
	}
	return v;
}

typedef Vec<3, float>	 Vec3;
typedef Vec<3, double>	 dVec3;
typedef Vec<3, bool>	 bVec3;
typedef Vec<3, int>		 iVec3;
typedef Vec<3, unsigned> uVec3;
}
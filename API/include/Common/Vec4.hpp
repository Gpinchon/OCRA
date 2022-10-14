#pragma once

#include <Common/Vec.hpp>

namespace OCRA {
template <typename T>
struct Vec<4, T> {
	typedef T value_type;
	typedef Vec<4, T> type;
	static constexpr auto length() { return 4; }
	Vec() : value{ 0, 0, 0, 0 } {}
	template<typename X, typename Y, typename Z, typename W>
	Vec(const X& a_X = X(0), const Y& a_Y = Y(0), const Z& a_Z = Z(0), const W& a_W = W(0))
		: x(a_X), y(a_Y), z(a_Z), w(a_W)
	{}
	auto& operator[] (const size_t a_Index) { return value[a_Index]; }
	auto& operator[] (const size_t a_Index) const { return value[a_Index]; }
	union {
		struct { value_type x, y, z, w; };
		struct { value_type r, g, b, a; };
		struct { value_type u, v, s, t; };
		value_type value[4];
	};
};

template<typename T>
auto operator*(const T& a_Scalar, Vec<4, T> const& a_V)
{
	return Vec<4, T>(
		a_Scalar * a_V.x,
		a_Scalar * a_V.y,
		a_Scalar * a_V.z,
		a_Scalar * a_V.w);
}

template<typename T>
auto operator*(Vec<4, T> const& a_V, const T& a_Scalar)
{
	return Vec<4, T>(
		a_V.x * a_Scalar,
		a_V.y * a_Scalar,
		a_V.z * a_Scalar,
		a_V.w * a_Scalar);
}

template<typename T>
auto operator/(const T& a_Scalar, Vec<4, T> const& a_V)
{
	return Vec<4, T>(
		a_Scalar / a_V.x,
		a_Scalar / a_V.y,
		a_Scalar / a_V.z,
		a_Scalar / a_V.w);
}

template<typename T>
auto operator/(Vec<4, T> const& a_V, const T& a_Scalar)
{
	return Vec<4, T>(
		a_V.x / a_Scalar,
		a_V.y / a_Scalar,
		a_V.z / a_Scalar,
		a_V.w / a_Scalar);
}

template<typename V1, typename V2>
auto operator+(Vec<4, V1> const& a_V1, Vec<4, V2> const& a_V2)
{
	return Vec<4, V1>(
		a_V1.x + a_V2.x,
		a_V1.y + a_V2.y,
		a_V1.z + a_V2.z,
		a_V1.w + a_V2.w);
}

template<typename V1, typename V2>
auto operator-(Vec<4, V1> const& a_V1, Vec<4, V2> const& a_V2)
{
	return Vec<4, V1>(
		a_V1.x - a_V2.x,
		a_V1.y - a_V2.y,
		a_V1.z - a_V2.z,
		a_V1.w - a_V2.w);
}

template<typename V1, typename V2>
auto operator*(Vec<4, V1> const& a_V1, Vec<4, V2> const& a_V2)
{
	return Vec<4, V1>(
		a_V1.x * a_V2.x,
		a_V1.y * a_V2.y,
		a_V1.z * a_V2.z,
		a_V1.w * a_V2.w);
}

template<typename V1, typename V2>
auto operator/(Vec<4, V1> const& a_V1, Vec<4, V2> const& a_V2)
{
	return Vec<4, V1>(
		a_V1.x / a_V2.x,
		a_V1.y / a_V2.y,
		a_V1.z / a_V2.z,
		a_V1.w / a_V2.w);
}

template<typename T>
auto Normalize(Vec<4, T> const& a_V)
{
	auto v = a_V;
	auto len = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);

	if (len != T(0))
	{
		v.x /= len;
		v.y /= len;
		v.z /= len;
		v.w /= len;
	}
	return v;
}

typedef Vec<4, float>	 Vec4;
typedef Vec<4, double>	 dVec4;
typedef Vec<4, bool>	 bVec4;
typedef Vec<4, int>		 iVec4;
typedef Vec<4, unsigned> uVec4;
}
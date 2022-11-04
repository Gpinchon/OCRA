#pragma once

#include <Common/Vec.hpp>
#include <Common/Vec2.hpp>

namespace OCRA {
template <typename T>
struct Vec<3, T> {
	typedef T value_type;
	typedef Vec<3, T> type;
	static constexpr auto length() { return 3; }
	Vec() : value{ 0, 0, 0 } {}
	explicit Vec(T a_S) : x(a_S), y(a_S), z(a_S) {}
	Vec(T a_X, T a_Y, T a_Z) : x(a_X), y(a_Y), z(a_Z) {}
	template<typename X, typename Y, typename Z>
	Vec(const X& a_X, const Y& a_Y, const Z& a_Z) : x(a_X), y(a_Y), z(a_Z) {}
	template<typename U>
	Vec(const Vec<3, U>& a_V) : Vec(a_V.x, a_V.y, a_V.z) {}
	template<typename U>
	Vec(const Vec<2, U>& a_V) : Vec(a_V.x, a_V.y) {}
	auto& operator[] (const size_t& a_Index) { return value[a_Index]; }
	auto& operator[] (const size_t& a_Index) const { return value[a_Index]; }
	union {
		value_type value[3];
		struct { value_type x, y, z; };
		struct { value_type r, g, b; };
		struct { value_type u, v, s; };
	};
};

typedef Vec<3, float>	 Vec3;
typedef Vec<3, double>	 dVec3;
typedef Vec<3, bool>	 bVec3;
typedef Vec<3, int>		 iVec3;
typedef Vec<3, unsigned> uVec3;
}
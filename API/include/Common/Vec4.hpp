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

typedef Vec<4, float>	 Vec4;
typedef Vec<4, double>	 dVec4;
typedef Vec<4, bool>	 bVec4;
typedef Vec<4, int>		 iVec4;
typedef Vec<4, unsigned> uVec4;
}
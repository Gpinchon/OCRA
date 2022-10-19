#pragma once

#include <Common/Vec.hpp>

namespace OCRA {
template <typename T>
struct Vec<2, T> {
	typedef T value_type;
	typedef Vec<2, T> type;
	static constexpr auto length() { return 2; }
	Vec() : value{ 0, 0 } {}
	explicit Vec(T a_S) : x(a_S), y(a_S) {}
	Vec(T a_X, T a_Y) : x(a_X), y(a_Y) {}
	template<typename X, typename Y>
	Vec<2, T>(const X& a_X, const Y& a_Y) : x(a_X), y(a_Y) {}
	auto& operator[](const size_t& a_Index) { return value[a_Index]; }
	auto& operator[](const size_t& a_Index) const { return value[a_Index]; }
	union {
		struct { value_type x, y; };
		struct { value_type r, g; };
		struct { value_type u, v; };
		value_type value[2];
	};
};

typedef Vec<2, float>	 Vec2;
typedef Vec<2, double>	 dVec2;
typedef Vec<2, int>		 iVec2;
typedef Vec<2, unsigned> uVec2;
typedef Vec<2, bool>     bVec2;
}
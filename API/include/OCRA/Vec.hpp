#pragma once

namespace OCRA {
template <unsigned L, typename T>
struct Vec;
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
        value_type value[2];
        struct { value_type x, y; };
        struct { value_type r, g; };
        struct { value_type u, v; };
    };
};
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
    template<typename U, typename W>
    Vec(const Vec<3, U> a_XYZ,  const W& a_W)
        : x(a_XYZ.x), y(a_XYZ.y), z(a_XYZ.z), w(a_W)
    {}
    auto& operator[] (const size_t a_Index) { return value[a_Index]; }
    auto& operator[] (const size_t a_Index) const { return value[a_Index]; }
    union {
        value_type value[4];
        struct { value_type x, y, z, w; };
        struct { value_type r, g, b, a; };
        struct { value_type u, v, s, t; };
    };
};

typedef Vec<2, float>    Vec2;
typedef Vec<2, double>   dVec2;
typedef Vec<2, int>      iVec2;
typedef Vec<2, unsigned> uVec2;
typedef Vec<2, bool>     bVec2;
typedef Vec<3, float>    Vec3;
typedef Vec<3, double>   dVec3;
typedef Vec<3, bool>     bVec3;
typedef Vec<3, int>      iVec3;
typedef Vec<3, unsigned> uVec3;
typedef Vec<4, float>    Vec4;
typedef Vec<4, double>   dVec4;
typedef Vec<4, bool>     bVec4;
typedef Vec<4, int>      iVec4;
typedef Vec<4, unsigned> uVec4;
}
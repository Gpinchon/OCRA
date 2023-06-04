#pragma once

namespace OCRA {
template <unsigned L, typename T>
struct Extent;
template <typename T>
struct Extent<2, T> {
    typedef T value_type;
    typedef Extent<2, T> type;
    constexpr auto length() { return 2; }
    constexpr Extent<2, T>(const T& a_Width = T(0), const T& a_Height = T(0))
        : width(a_Width)
        , height(a_Height)
    {
    }
    struct {
        T width, height;
    };
};
template <typename T>
struct Extent<3, T> {
    typedef T value_type;
    typedef Extent<3, T> type;
    constexpr auto length() { return 3; }
    constexpr Extent(const T& a_Width = T(0), const T& a_Height = T(0), const T& a_Depth = T(0))
        : width(a_Width)
        , height(a_Height)
        , depth(a_Depth)
    {
    }
    template <typename W, typename H, typename D>
    constexpr Extent(const W& a_Width, const H& a_Height, const D& a_Depth)
        : Extent(T(a_Width), T(a_Height), T(a_Depth))
    {
    }
    template <typename W, typename H>
    constexpr Extent(const W& a_Width, const H& a_Height)
        : Extent(T(a_Width), T(a_Height))
    {
    }
    template <typename W>
    constexpr Extent(const W& a_Width)
        : Extent(T(a_Width))
    {
    }
    struct {
        T width, height, depth;
    };
};

typedef Extent<2, float> Extent2D;
typedef Extent<2, int> iExtent2D;
typedef Extent<2, unsigned> uExtent2D;
typedef Extent<3, float> Extent3D;
typedef Extent<2, int> iExtent3D;
typedef Extent<2, unsigned> uExtent3D;
}

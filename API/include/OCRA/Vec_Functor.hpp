#pragma once

#include <OCRA/Vec.hpp>

namespace OCRA {
template<unsigned L, typename T, typename R>
struct VecFunctor
{
    template<typename Func>
    static inline auto call(Vec<L, T> const& a_Vec)
    {
        Vec<L, R> result;
        for (auto i = 0u; i < L; ++i)
            result[i] = Func{}(a_Vec[i]);
        return result;
    }
};

template<unsigned L, typename T, typename U, typename R>
struct VecFunctor1
{
    template<typename Func>
    static inline auto call(Vec<L, T> const& a_X, Vec<L, U> const& a_Y)
    {
        Vec<L, R> result;
        for (auto i = 0u; i < L; ++i)
            result[i] = Func{}(a_X[i], a_Y[i]);
        return result;
    }
    template<typename Func>
    static inline auto call(Vec<L, T> const& a_X, U const& a_Y)
    {
        Vec<L, R> result;
        for (auto i = 0u; i < L; ++i)
            result[i] = Func{}(a_X[i], a_Y);
        return result;
    }
    template<typename Func>
    static inline auto call(T const& a_X, Vec<L, U> const& a_Y)
    {
        Vec<L, R> result;
        for (auto i = 0u; i < L; ++i)
            result[i] = Func{}(a_X, a_Y[i]);
        return result;
    }
};
}
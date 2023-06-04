#pragma once

#include <OCRA/Vec.hpp>

namespace OCRA {
template <size_t C, size_t R, typename T>
struct Mat;

template <typename T>
struct Mat<4, 4, T> {
    typedef T value_type;
    typedef Mat<4, 4, T> type;
    typedef Vec<4, T> col_type;
    typedef Vec<4, T> row_type;

    Mat()
        : value {
            col_type(1, 0, 0, 0),
            col_type(0, 1, 0, 0),
            col_type(0, 0, 1, 0),
            col_type(0, 0, 0, 1)
        }
    {
    }

    template <typename V1, typename V2, typename V3, typename V4>
    Mat(col_type const& a_V1,
        col_type const& a_V2,
        col_type const& a_V3,
        col_type const& a_V4)
    {
        value[0] = a_V1;
        value[1] = a_V2;
        value[2] = a_V3;
        value[3] = a_V4;
    }
    auto& operator[](size_t const& a_Col)
    {
        return value[a_Col];
    }
    auto& operator[](size_t const& a_Col) const
    {
        return value[a_Col];
    }
    template <typename T>
    static auto Rotate(Mat<4, 4, T> const& a_M, T a_Angle, Vec<3, T> const& a_V)
    {
        T const a = a_Angle;
        T const c = cos(a);
        T const s = sin(a);

        auto axis(normalize(a_V));
        auto temp((T(1) - c) * axis);

        Mat<4, 4, T> Rotate;
        Rotate[0][0] = c + temp[0] * axis[0];
        Rotate[0][1] = temp[0] * axis[1] + s * axis[2];
        Rotate[0][2] = temp[0] * axis[2] - s * axis[1];

        Rotate[1][0] = temp[1] * axis[0] - s * axis[2];
        Rotate[1][1] = c + temp[1] * axis[1];
        Rotate[1][2] = temp[1] * axis[2] + s * axis[0];

        Rotate[2][0] = temp[2] * axis[0] + s * axis[1];
        Rotate[2][1] = temp[2] * axis[1] - s * axis[0];
        Rotate[2][2] = c + temp[2] * axis[2];

        Mat<4, 4, T> Result;
        Result[0] = a_M[0] * Rotate[0][0] + a_M[1] * Rotate[0][1] + a_M[2] * Rotate[0][2];
        Result[1] = a_M[0] * Rotate[1][0] + a_M[1] * Rotate[1][1] + a_M[2] * Rotate[1][2];
        Result[2] = a_M[0] * Rotate[2][0] + a_M[1] * Rotate[2][1] + a_M[2] * Rotate[2][2];
        Result[3] = a_M[3];
        return Result;
    }

private:
    col_type value[4];
};
typedef Mat<4, 4, float> Mat4x4;
}

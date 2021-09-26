/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:27:47
*/
#pragma once

#include <bitset>

namespace OCRA::Blend {
using ColorMask = std::bitset<4>;
constexpr ColorMask R = 0b1000;
constexpr ColorMask G = 0b0100;
constexpr ColorMask B = 0b0010;
constexpr ColorMask A = 0b0001;
constexpr ColorMask RGBA = 0b1111;
constexpr ColorMask None = 0b0000;
enum class Operation {
    Add,
    Substract,
    ReverseSubstract,
    Min,
    Max,
    MaxValue
};
enum class Factor {
    Zero,
    One,
    SrcColor,
    OneMinusSrcColor,
    DstColor,
    OneMinusDstColor,
    SrcAlpha,
    OneMinusSrcAlpha,
    DstAlpha,
    OneMinusDstAlpha,
    ConstantColor,
    OneMinusConstantColor,
    ConstantAlpha,
    OneMinusConstantAlpha,
    SrcAlphaSaturate,
    Src1Color,
    OneMinusSrc1Color,
    Src1Alpha,
    OneMinusSrc1Alpha,
    MaxValue
};
}
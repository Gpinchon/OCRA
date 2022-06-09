/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:45
*/
#pragma once

#include <Common/ColorValue.hpp>
#include <Common/DepthStencilValue.hpp>

namespace OCRA
{
struct ClearValue {
    ColorValue        color{};
    DepthStencilValue depthStencil{};
};
}
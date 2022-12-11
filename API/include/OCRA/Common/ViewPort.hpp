/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:27:19
*/
#pragma once

#include <OCRA/Common/Rect2D.hpp>
#include <OCRA/Common/DepthRange.hpp>

namespace OCRA {
struct ViewPort {
    Rect2D              rect{ 0, 0, 0, 0 };
    DepthRange<double>  depthRange{ 0, 1 };
};
}
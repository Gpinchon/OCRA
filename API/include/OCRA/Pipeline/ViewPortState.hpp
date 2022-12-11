/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:56
*/
#pragma once

#include <OCRA/Common/Rect2D.hpp>
#include <OCRA/Common/ViewPort.hpp>

#include <vector>

namespace OCRA::Pipeline::ViewPortState {
struct Info {
    std::vector<ViewPort> viewPorts{};
    std::vector<Rect2D>   scissors{};
};
}
/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:56
*/
#pragma once

#include <Common/Rect2D.hpp>
#include <ViewPort.hpp>
#include <Scalar.hpp>

#include <array>

namespace OCRA::Pipeline::ViewPortState {
constexpr auto MaxViewPorts = 64;
struct Info {
	Uint8 viewPortsCount{ 0 };
    std::array<ViewPort::Info, MaxViewPorts> viewPorts;
    std::array<Rect2D<Int32>, MaxViewPorts> scissors;
};
}
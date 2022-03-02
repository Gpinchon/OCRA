/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:56
*/
#pragma once

#include <Pipeline/ViewPortState.hpp>
#include <Common/Rect2D.hpp>

#include <array>

#include <GL/glew.h>

namespace OCRA::Pipeline::ViewPortState {
inline auto Compile(const Device::Handle& a_Device, const Info& a_Info)
{
	return [info = a_Info]() {
		glViewportArrayv(
			0, //first
			a_Info.viewPortsCount,
			&a_Info.viewPorts.front().rect.offset.x);
		glDepthRangeArrayv(
			0, //first
			a_Info.viewPortsCount,
			&a_Info.viewPorts.front().depthBounds.near);
		glScissorArrayv(
			0,
			a_Info.viewPortsCount,
			&a_Info.scissors.offset.x);
	};
}
}
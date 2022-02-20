/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:56
*/
#pragma once

#include <Pipeline/ViewPortState.hpp>
#include <Rect2D.hpp>

#include <array>

#include <GL/glew.h>

namespace OCRA::Pipeline::ViewPortState {
template<typename T>
struct Rect {
	Rect() = default;
	Rect(const Rect2D& a_Info)
	: x(a_Info.offset.x)
	, y(a_Info.offset.y)
	, w(a_Info.extent.width)
	, h(a_Info.extent.height)
	{}
	T x, y; //left, bottom
	T w, h; //width, height
};
struct DepthRange {
	DepthRange() = default;
	DepthRange(const ViewPort::Info& a_Info)
	: near(a_Info.minDepth)
	, far(a_Info.maxDepth)
	{}
	GLdouble near, far;
};
inline auto Compile(const Device::Handle& a_Device, const Info& a_Info)
{
	const auto viewPortsCount(a_Info.viewPortsCount);
	const auto viewPorts([a_Info]() {
		std::array<Rect<GLfloat>, MaxViewPorts> vp;
		for (auto i = 0u; i < a_Info.viewPortsCount; ++i)
			vp.at(i) = Rect<GLfloat>(a_Info.viewPorts.at(i).rect);
		return vp;
	}());
	const auto viewPortsDepthRange([a_Info]() {
		std::array<DepthRange, MaxViewPorts> vp;
		for (auto i = 0u; i < a_Info.viewPortsCount; ++i)
			vp.at(i) = DepthRange(a_Info.viewPorts.at(i));
		return vp;
	}());
	const auto scissors([a_Info]() {
		std::array<Rect<GLint>, MaxViewPorts> vp;
		for (auto i = 0u; i < a_Info.viewPortsCount; ++i)
			vp.at(i) = Rect<GLint>(a_Info.scissors.at(i));
		return vp;
	}());
	return [viewPortsCount, viewPorts, viewPortsDepthRange, scissors]() {
		glViewportArrayv(
			0, //first
			viewPortsCount,
			(GLfloat*)viewPorts.data());
		glDepthRangeArrayv(
			0, //first
			viewPortsCount,
			(GLdouble*)viewPortsDepthRange.data());
		glScissorArrayv(
			0,
			viewPortsCount,
			(GLint*)scissors.data());
	};
}
}
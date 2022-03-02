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
	const auto viewportsRects = [=]() {
		std::vector<Rect2D> vec;
		for (auto i = 0; i < a_Info.viewPortsCount; ++i)
			vec.push_back(a_Info.viewPorts.at(i).rect);
		return vec;
	}();
	const auto viewportsDepthRanges = [=]() {
		std::vector<DepthBounds<GLdouble>> vec;
		for (auto i = 0; i < a_Info.viewPortsCount; ++i)
			vec.push_back(a_Info.viewPorts.at(i).depthBounds);
		return vec;
	}();
	const auto viewportsScissors = [=]() {
		std::vector<Rect<2, GLint>> vec;
		for (auto i = 0; i < a_Info.viewPortsCount; ++i)
			vec.push_back(a_Info.scissors.at(i));
		return vec;
	}();
	return [
		viewportsRects, viewportsDepthRanges, viewportsScissors
	]() {
		glViewportArrayv(
			0, //first
			viewportsRects.size(),
			&viewportsRects.front().offset.x);
		glDepthRangeArrayv(
			0, //first
			viewportsDepthRanges.size(),
			&viewportsDepthRanges.front().min);
		glScissorArrayv(
			0,
			viewportsScissors.size(),
			&viewportsScissors.front().offset.x);
	};
}
}
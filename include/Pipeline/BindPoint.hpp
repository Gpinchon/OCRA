/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:28:15
*/
#pragma once

namespace OCRA::Pipeline {
enum class BindPoint {
	Unknown = -1,
	Graphics,
	Compute,
	RayTracing
};
}
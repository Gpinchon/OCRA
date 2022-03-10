/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:45
*/
#pragma once

#include <bitset>


namespace OCRA::Pipeline
{
enum class BindingPoint
{
	Unknown = -1,
	Graphics,
	Compute,
	RayTracing, //might be unavailable
	MaxValue
};
}
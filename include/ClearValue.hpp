/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:45
*/
#pragma once

#include <Scalar.hpp>

namespace OCRA
{
struct ClearValue {
	union ColorValue {
		float float32[4]{ 0, 0, 0, 0 };
		Int32 int32[4];
		Uint32 uint32[4];
	} color;
	struct DepthStencilValue {
		float depth{ 0 };
		Uint32 stencil{ 0 };
	} depthStencil;
};
}
/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:45
*/
#pragma once

namespace OCRA
{
struct ClearValue {
	union ColorValue {
		float float32[4]{ 0, 0, 0, 0 };
		int32_t int32[4];
		uint32_t uint32[4];
	} color;
	struct DepthStencilValue {
		float depth{ 0 };
		uint32_t stencil{ 0 };
	} depthStencil;
};
}
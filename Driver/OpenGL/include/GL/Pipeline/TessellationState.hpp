/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:30
*/

#include <Pipeline/TessellationState.hpp>

namespace OCRA::Pipeline::TessellationState {
inline auto Compile(const Device::Handle& a_Device, const Info& a_Info)
{
	return [patchControlPoints = a_Info.patchControlPoints]() {
		glPatchParameteri(GL_PATCH_VERTICES, patchControlPoints);
	};
}
}
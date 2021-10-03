/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:30
*/

#include <Pipeline/TessellationState.hpp>

namespace OCRA::Pipeline::TessellationState {
struct Compile {
	Compile(const Device::Handle& a_Device, const Info& a_Info)
	    : patchControlPoints(a_Info.patchControlPoints)
	{}
	const GLuint patchControlPoints;
	void operator()(const Device::Handle& a_Device)
	{
		glPatchParameteri(GL_PATCH_VERTICES, patchControlPoints);
	}
};
}
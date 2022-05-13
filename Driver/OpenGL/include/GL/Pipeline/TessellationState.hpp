/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:25:30
*/

#include <Pipeline/TessellationState.hpp>

namespace OCRA::Pipeline::DynamicState {
struct Info;
}



namespace OCRA::Pipeline::TessellationState {
inline std::function<void(Command::Buffer::ExecutionState&)> Compile(const Device::Handle& a_Device, const Info& a_Info, const DynamicState::Info&)
{
	if (a_Info.patchControlPoints > 0) return[patchControlPoints = a_Info.patchControlPoints](Command::Buffer::ExecutionState&) {
		glPatchParameteri(GL_PATCH_VERTICES, patchControlPoints);
	};
	else return [](Command::Buffer::ExecutionState&) {};
}
}
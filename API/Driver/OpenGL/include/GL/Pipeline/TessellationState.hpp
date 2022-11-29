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



namespace OCRA::Pipeline::TessellationState
{
struct Compile
{
    Compile(const Device::Handle& a_Device, const Info& a_Info, const DynamicState::Info&)
        : patchControlPoints(a_Info.patchControlPoints)
    {}
    void operator()(Command::Buffer::ExecutionState& a_ExecutionState) const
    {
        if (patchControlPoints > 0)
            glPatchParameteri(GL_PATCH_VERTICES, patchControlPoints);
    }
    const uint32_t patchControlPoints;
};
}
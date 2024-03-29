/*
 * @Author: gpinchon
 * @Date:   2021-09-26 00:00:00
 * @Last Modified by:   gpinchon
 * @Last Modified time: 2021-09-26 14:25:30
 */
#pragma once

#include <OCRA/Core.hpp>

namespace OCRA::Pipeline {
struct CompileTessellationState {
    CompileTessellationState(const Device::Handle& a_Device, const PipelineTessellationState& a_Info, const PipelineDynamicState&)
        : patchControlPoints(a_Info.patchControlPoints)
    {
    }
    void operator()(Command::Buffer::ExecutionState& a_ExecutionState) const
    {
        if (patchControlPoints > 0)
            glPatchParameteri(GL_PATCH_VERTICES, patchControlPoints);
    }
    const uint32_t patchControlPoints;
};
}

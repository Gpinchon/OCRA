#pragma once

#include <OCRA/Core.hpp>

#include <GL/Command/ExecutionState.hpp>
#include <GL/glew.h>

namespace OCRA::Pipeline {
inline auto GetGLPrimitiveTopology(const PrimitiveTopology& a_Topology)
{
    switch (a_Topology) {
    case PrimitiveTopology::Unknown:
        return GL_NONE;
    case PrimitiveTopology::PointList:
        return GL_POINTS;
    case PrimitiveTopology::LineList:
        return GL_LINES;
    case PrimitiveTopology::LineStrip:
        return GL_LINE_STRIP;
    case PrimitiveTopology::TriangleList:
        return GL_TRIANGLES;
    case PrimitiveTopology::TriangleStrip:
        return GL_TRIANGLE_STRIP;
    case PrimitiveTopology::TriangleFan:
        return GL_TRIANGLE_FAN;
    case PrimitiveTopology::LineListWithAdjacency:
        return GL_LINES_ADJACENCY;
    case PrimitiveTopology::LineStripWithAdjacency:
        return GL_LINE_STRIP_ADJACENCY;
    case PrimitiveTopology::TriangleListWithAdjacency:
        return GL_TRIANGLES_ADJACENCY;
    case PrimitiveTopology::TriangleStripWithAdjacency:
        return GL_TRIANGLE_STRIP_ADJACENCY;
    case PrimitiveTopology::PatchList:
        return GL_PATCHES;
    default:
        throw std::runtime_error("Unknown Primitive Topology");
    }
}
struct CompileInputAssemblyState
{
    CompileInputAssemblyState(const Device::Handle& a_Device, const PipelineInputAssemblyState& a_Info, const PipelineDynamicState& a_DynamicState)
        : dynamicPrimitiveRestartEnable(a_DynamicState.Contains(DynamicState::PrimitiveRestartEnable))
        , dynamicPrimitiveTopology(a_DynamicState.Contains(DynamicState::PrimitiveTopology))
        , primitiveRestartEnable(a_Info.primitiveRestartEnable)
        , primitiveTopology(GetGLPrimitiveTopology(a_Info.topology))
    {}
    void operator()(Command::Buffer::ExecutionState& a_ExecutionState) const
    {
        if (dynamicPrimitiveRestartEnable) {
            a_ExecutionState.dynamicStates.primitiveRestartEnable ? glEnable(GL_PRIMITIVE_RESTART) : glDisable(GL_PRIMITIVE_RESTART);
        }
        else primitiveRestartEnable ? glEnable(GL_PRIMITIVE_RESTART) : glDisable(GL_PRIMITIVE_RESTART);
        if (dynamicPrimitiveTopology) {
            a_ExecutionState.primitiveTopology = a_ExecutionState.dynamicStates.primitiveTopology;
        }
        else a_ExecutionState.primitiveTopology = primitiveTopology;
    }
    const bool   dynamicPrimitiveRestartEnable;
    const bool   dynamicPrimitiveTopology;
    const bool   primitiveRestartEnable;
    const GLenum primitiveTopology;
};
}
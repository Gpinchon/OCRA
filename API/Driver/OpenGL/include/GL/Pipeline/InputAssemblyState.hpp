#pragma once

#include <OCRA/Pipeline/InputAssemblyState.hpp>
#include <OCRA/Primitive/Topology.hpp>
#include <OCRA/Pipeline/DynamicState.hpp>

#include <GL/Command/ExecutionState.hpp>
#include <GL/glew.h>

namespace OCRA::Pipeline::InputAssemblyState {
inline auto GetGLTopology(const Primitive::Topology& a_Topology)
{
    switch (a_Topology) {
    case Primitive::Topology::Unknown:
        return GL_NONE;
    case Primitive::Topology::PointList:
        return GL_POINTS;
    case Primitive::Topology::LineList:
        return GL_LINES;
    case Primitive::Topology::LineStrip:
        return GL_LINE_STRIP;
    case Primitive::Topology::TriangleList:
        return GL_TRIANGLES;
    case Primitive::Topology::TriangleStrip:
        return GL_TRIANGLE_STRIP;
    case Primitive::Topology::TriangleFan:
        return GL_TRIANGLE_FAN;
    case Primitive::Topology::LineListWithAdjacency:
        return GL_LINES_ADJACENCY;
    case Primitive::Topology::LineStripWithAdjacency:
        return GL_LINE_STRIP_ADJACENCY;
    case Primitive::Topology::TriangleListWithAdjacency:
        return GL_TRIANGLES_ADJACENCY;
    case Primitive::Topology::TriangleStripWithAdjacency:
        return GL_TRIANGLE_STRIP_ADJACENCY;
    case Primitive::Topology::PatchList:
        return GL_PATCHES;
    default:
        throw std::runtime_error("Unknown Primitive Topology");
    }
}
struct Compile
{
    Compile(const Device::Handle& a_Device, const InputAssemblyState::Info& a_Info, const DynamicState::Info& a_DynamicState)
        : dynamicPrimitiveRestartEnable(a_DynamicState.Contains(DynamicState::State::PrimitiveRestartEnable))
        , dynamicPrimitiveTopology(a_DynamicState.Contains(DynamicState::State::PrimitiveTopology))
        , primitiveRestartEnable(a_Info.primitiveRestartEnable)
        , primitiveTopology(GetGLTopology(a_Info.topology))
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
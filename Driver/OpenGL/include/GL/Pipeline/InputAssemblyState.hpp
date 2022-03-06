#pragma once

#include <Pipeline/InputAssemblyState.hpp>
#include <Primitive/Topology.hpp>
#include <Pipeline/DynamicState.hpp>

#include <GL/Command/Buffer/ExecutionState.hpp>
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
inline std::function<void(Command::Buffer::ExecutionState&)> Compile(const Device::Handle& a_Device, const InputAssemblyState::Info& a_Info, const DynamicState::Info& a_DynamicState)
{
	const auto dynamicPrimitiveRestartEnable = a_DynamicState.Contains(DynamicState::State::PrimitiveRestartEnable);
	const auto dynamicPrimitiveTopology = a_DynamicState.Contains(DynamicState::State::PrimitiveTopology);
	if (dynamicPrimitiveRestartEnable && dynamicPrimitiveTopology)
		return [](Command::Buffer::ExecutionState& a_ExecutionState){
			a_ExecutionState.dynamicStates.primitiveRestartEnable ? glEnable(GL_PRIMITIVE_RESTART) : glDisable(GL_PRIMITIVE_RESTART);
			a_ExecutionState.renderPass.primitiveTopology = a_ExecutionState.dynamicStates.primitiveTopology;
		};
	else if (dynamicPrimitiveRestartEnable)
		return [
			primitivetopology(GetGLTopology(a_Info.topology))
		](Command::Buffer::ExecutionState& a_ExecutionState){
			a_ExecutionState.dynamicStates.primitiveRestartEnable ? glEnable(GL_PRIMITIVE_RESTART) : glDisable(GL_PRIMITIVE_RESTART);
			a_ExecutionState.renderPass.primitiveTopology = primitivetopology;
		};
	else if (dynamicPrimitiveTopology)
		return [
			primitiveRestartEnable(a_Info.primitiveRestartEnable)
		](Command::Buffer::ExecutionState& a_ExecutionState){
			primitiveRestartEnable ? glEnable(GL_PRIMITIVE_RESTART) : glDisable(GL_PRIMITIVE_RESTART);
			a_ExecutionState.renderPass.primitiveTopology = a_ExecutionState.dynamicStates.primitiveTopology;
		};
	else return [
			primitivetopology(GetGLTopology(a_Info.topology)),
			primitiveRestartEnable(a_Info.primitiveRestartEnable)
		](Command::Buffer::ExecutionState& a_ExecutionState){
			primitiveRestartEnable ? glEnable(GL_PRIMITIVE_RESTART) : glDisable(GL_PRIMITIVE_RESTART);
			a_ExecutionState.renderPass.primitiveTopology = primitivetopology;
		};
}
}
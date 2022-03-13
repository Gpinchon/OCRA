#pragma once

#include <Command/Buffer.hpp>
#include <Handle.hpp>
#include <bitset>

HANDLE(OCRA::Pipeline);

namespace OCRA::Pipeline
{
enum class BindingPoint
{
	Unknown = -1,
	Graphics,
	Compute,
	RayTracing, //might be unavailable
	MaxValue
};
}

namespace OCRA::Command
{
//Bind the specified Pipeline to the specified Command Buffer at the specified Binding Point
void BindPipeline(
	const Command::Buffer::Handle& a_CommandBuffer,
	const Pipeline::BindingPoint& a_BindingPoint,
	const Pipeline::Handle& a_Pipeline);
}

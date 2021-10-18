/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/
#pragma once

#include <Handle.hpp>
#include <Pipeline/BindingPoint.hpp>

namespace OCRA::Command::Pipeline
{
//Bind the specified Pipeline to the specified Command Buffer at the specified Binding Point
void Bind(
	const Command::Buffer::Handle& a_CommandBuffer,
	const Pipeline::BindingPoint& a_BindingPoint,
	const Pipeline::Handle& a_Pipeline);
}
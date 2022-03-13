#pragma once

#include <Pipeline/Pipeline.hpp>

#include <functional>

namespace OCRA::Command::Buffer {
struct ExecutionState;
}

namespace OCRA::Pipeline {
struct Impl {
	Impl(const BindingPoint& a_BindingPoint) : bindingPoint(a_BindingPoint) {};
	virtual void Execute(const Device::Handle& a_Device, Command::Buffer::ExecutionState& a_ExecutionState) = 0;
	const BindingPoint bindingPoint;
};
void Execute(const Device::Handle& a_Device, const Handle& a_Handle, Command::Buffer::ExecutionState& a_ExecutionState) {
	a_Handle->Execute(a_Device, a_ExecutionState);
}
}
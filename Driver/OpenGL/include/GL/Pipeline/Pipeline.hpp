#pragma once

#include <Handle.hpp>

#include <functional>

namespace OCRA::Command::Buffer {
struct ExecutionState;
}

namespace OCRA::Pipeline {
struct Base {
	virtual void Execute(const Device::Handle& a_Device, const Handle& a_Handle, Command::Buffer::ExecutionState& a_ExecutionState);
};
Base& Get(const Handle& a_Pipeline);
Handle Create(const Device::Handle& a_Device, std::function<Base*()> a_Allocator);
void Destroy(const Device::Handle& a_Device, const Handle& a_Handle);
void Execute(const Device::Handle& a_Device, const Handle& a_Handle, Command::Buffer::ExecutionState& a_ExecutionState);
}
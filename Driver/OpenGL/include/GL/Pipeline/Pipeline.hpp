#pragma once

#include <Handle.hpp>

#include <functional>

namespace OCRA::Pipeline {
struct Base {
	virtual std::function<void()> Compile(const Device::Handle& a_Device) = 0;
};
Base& Get(const Handle& a_Pipeline);
Handle Create(const Device::Handle& a_Device, std::function<Base*()> a_Allocator);
void Destroy(const Device::Handle& a_Device, const Handle& a_Handle);
std::function<void()> Compile(const Device::Handle& a_Device, const Handle& a_Pipeline);
}
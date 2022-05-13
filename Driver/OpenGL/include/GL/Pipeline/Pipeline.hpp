#pragma once

#include <Pipeline/Pipeline.hpp>

#include <GL/WeakHandle.hpp>

#include <functional>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Command::Buffer {
struct ExecutionState;
}
namespace OCRA::Pipeline {
struct Impl {
	Impl(const Device::Handle& a_Device, const BindingPoint& a_BindingPoint)
		: device(a_Device)
		, bindingPoint(a_BindingPoint)
	{};
	std::function<void(Command::Buffer::ExecutionState&)> Apply;
	const Device::WeakHandle device;
	const BindingPoint		 bindingPoint;
};
}
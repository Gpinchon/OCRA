#pragma once

#include <Pipeline/Pipeline.hpp>

#include <functional>

namespace OCRA::Command::Buffer {
struct ExecutionState;
}
namespace OCRA::Pipeline {
struct Impl {
	Impl(const BindingPoint& a_BindingPoint, const Layout::Handle& a_Layout)
		: layout(a_Layout)
		, bindingPoint(a_BindingPoint)
	{};
	std::function<void(Command::Buffer::ExecutionState&)> Apply;
	const Layout::Handle     layout;
	const BindingPoint		 bindingPoint;
};
}
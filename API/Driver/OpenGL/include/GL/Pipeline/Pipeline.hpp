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
	virtual void Apply(Command::Buffer::ExecutionState&) const = 0;
	const Layout::Handle     layout;
	const BindingPoint		 bindingPoint;
};
}
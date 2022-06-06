#pragma once

#include <Handle.hpp>
#include <Descriptor/SetLayout.hpp>

#include <GL/WeakHandle.hpp>

#include <algorithm>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);



namespace OCRA::Descriptor::SetLayout
{
static inline auto CreateDirectIndexedLayout(const std::vector<Binding>& a_Bindings)
{
	std::vector<Binding> bindings;
	size_t size = 0;
	for (const auto& binding : a_Bindings) {
		size = std::max(size, binding.binding);
	}
	bindings.resize(size);
	for (const auto& binding : a_Bindings) {
		bindings.at(binding.binding) = binding;
	}
	return bindings;
}
struct Impl
{
	Impl(const Device::Handle& a_Device, const Info& a_Info)
		: device(a_Device)
		, bindings(CreateDirectIndexedLayout(a_Info.bindings))
	{}
	const Device::WeakHandle device;
	const std::vector<Binding> bindings;
};
}
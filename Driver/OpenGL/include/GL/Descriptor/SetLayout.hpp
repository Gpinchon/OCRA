#pragma once

#include <Handle.hpp>
#include <Descriptor/SetLayout.hpp>

#include <GL/WeakHandle.hpp>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

static inline auto CreateDirectIndexedLayout(const std::vector<LayoutBinding>& a_Bindings)
{
	std::vector<LayoutBinding> bindings;
	size_t size = 0;
	for (const auto& binding : a_Bindings) {
		size = max(size, binding.binding);
	}
	bindings.resize(size);
	for (const auto& binding : a_Bindings) {
		bindings.at(binding.binding) = binding;
	}
	return bindings;
}

namespace OCRA::Descriptor::SetLayout
{
struct Impl
{
	Impl(const Device::Handle& a_Device, const std::vector<LayoutBinding>& a_Bindings)
		: device(a_Device)
		, bindings(CreateDirectIndexedLayout(a_Bindings))
	{}
	const Device::WeakHandle device;
	const std::vector<LayoutBinding> bindings;
};
}
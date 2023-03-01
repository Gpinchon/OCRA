#pragma once

#include <OCRA/Core.hpp>

#include <GL/DescriptorBinding.hpp>

#include <algorithm>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Descriptor::SetLayout
{
static inline auto CreateDirectIndexedLayout(const std::vector<DescriptorSetLayoutBinding>& a_Bindings)
{
    std::vector<DescriptorSetLayoutBinding> bindings;
    size_t size = 0;
    for (const auto& binding : a_Bindings) {
        size = std::max(size, size_t(binding.binding + 1));
    }
    bindings.resize(size);
    for (const auto& binding : a_Bindings) {
        bindings.at(binding.binding) = binding;
    }
    return bindings;
}
struct Impl
{
    Impl(const Device::Handle& a_Device, const CreateDescriptorSetLayoutInfo& a_Info)
        : device(a_Device)
        , bindings(CreateDirectIndexedLayout(a_Info.bindings))
    {}
    const Device::WeakHandle device;
    const std::vector<DescriptorSetLayoutBinding> bindings;
};
}
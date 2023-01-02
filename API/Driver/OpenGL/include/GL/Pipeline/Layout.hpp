#pragma once

#include <OCRA/Pipeline/Layout.hpp>
#include <OCRA/Descriptor/Set.hpp>
#include <OCRA/Descriptor/Type.hpp>

#include <array>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Pipeline::Layout
{
class Impl
{
public:
    Impl(const Info& a_Info);
    bool IsCompatibleWith(const Handle& a_Layout);

    auto GetType(uint32_t a_BindingIndex) const {
        return descriptorSet.at(a_BindingIndex).type;
    }
    auto GetOffset(uint32_t a_BindingIndex) const {
        return descriptorSet.at(a_BindingIndex).offset;
    }
    auto GetDynamicOffset(uint32_t a_BindingIndex) const {
        return descriptorSet.at(a_BindingIndex).dynamicOffset;
    }
    auto GetDescriptorCount(uint32_t a_BindingIndex) const {
        return descriptorSet.at(a_BindingIndex).descriptorCount;
    }

private:
    struct Binding {
        Descriptor::Type type;
        uint32_t offset;
        uint32_t dynamicOffset;
        uint32_t descriptorCount;
    };
    using DescriptorSet = std::vector<Binding>;

    const uint32_t descriptorSetCount = 0;
    DescriptorSet descriptorSet;
    std::vector<PushConstantRange> pushConstantRanges;
};
}

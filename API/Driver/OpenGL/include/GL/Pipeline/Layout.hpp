#pragma once

#include <OCRA/Core.hpp>

#include <array>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Pipeline::Layout
{
class Impl
{
public:
    Impl(const CreatePipelineLayoutInfo& a_Info);
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
        DescriptorType type;
        uint32_t offset;
        uint32_t dynamicOffset;
        uint32_t descriptorCount;
    };
    std::vector<Binding> descriptorSet;
    std::vector<PushConstantRange> pushConstantRanges;
};
}

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

    auto GetType(uint32_t a_SetIndex, uint32_t a_BindingIndex) const {
        return descriptorSets.at(a_SetIndex).bindings.at(a_BindingIndex).type;
    }
    auto GetOffset(uint32_t a_SetIndex, uint32_t a_BindingIndex) const {
        return descriptorSets.at(a_SetIndex).bindings.at(a_BindingIndex).offset;
    }
    auto GetDynamicOffset(uint32_t a_SetIndex, uint32_t a_BindingIndex) const {
        return descriptorSets.at(a_SetIndex).bindings.at(a_BindingIndex).dynamicOffset;
    }
    auto GetDescriptorCount(uint32_t a_SetIndex, uint32_t a_BindingIndex) const {
        return descriptorSets.at(a_SetIndex).bindings.at(a_BindingIndex).descriptorCount;
    }

private:
    struct Binding {
        Descriptor::Type type;
        uint32_t offset;
        uint32_t dynamicOffset;
        uint32_t descriptorCount;
    };
    struct DescriptorSet {
        std::vector<Binding> bindings;
    };

    const uint32_t descriptorSetCount = 0;
    std::array<DescriptorSet, OCRA_GL_MAX_BOUND_DESCRIPTOR_SETS> descriptorSets;
    std::vector<PushConstantRange> pushConstantRanges;
};
}

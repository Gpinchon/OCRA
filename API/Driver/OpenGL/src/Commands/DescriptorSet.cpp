////////////////////////////////////////////////////////////////////////////////
//  Buffer commands
////////////////////////////////////////////////////////////////////////////////

#include <GL/Common/Assert.hpp>
#include <GL/Command/Buffer.hpp>
#include <GL/Command/ExecutionState.hpp>
#include <GL/Pipeline/Pipeline.hpp>
#include <GL/Pipeline/Layout.hpp>

namespace OCRA::Command{
struct BindDescriptorSetCommand : CommandI {
    BindDescriptorSetCommand(
        const Pipeline::BindingPoint&   a_BindingPoint,
        const Pipeline::Layout::Handle& a_Layout,
        const Descriptor::Set::Handle&  a_DescriptorSet,
        const uint32_t                  a_DynamicOffset)
        : layout(a_Layout)
        , descriptorSet(a_DescriptorSet)
        , bindingPoint(uint8_t(a_BindingPoint))
        , dynamicOffset(a_DynamicOffset)
    {}
    virtual void operator()(Buffer::ExecutionState& a_ExecutionState) {
        auto& pipelineState = a_ExecutionState.pipelineState.at(bindingPoint);
        pipelineState.descriptorSet = descriptorSet;
        pipelineState.descriptorDynamicOffset = dynamicOffset;
    }
    const Pipeline::Layout::Handle layout;
    const Descriptor::Set::Handle  descriptorSet;
    const uint8_t  bindingPoint;
    const uint32_t dynamicOffset;
};

struct PushDescriptorSetCommand : CommandI {
    PushDescriptorSetCommand(
        const Pipeline::BindingPoint& a_BindingPoint,
        const Pipeline::Layout::Handle& a_Layout,
        const size_t& a_WriteCount,
        const Descriptor::Set::WriteOperation* a_Writes)
        : layout(a_Layout)
        , bindingPoint(uint8_t(a_BindingPoint))
        , writeCount(a_WriteCount)
    {
        OCRA_ASSERT(writeCount <= 256);
        std::copy(a_Writes, a_Writes + a_WriteCount, writes.data());
    }
    virtual void operator()(Buffer::ExecutionState& a_ExecutionState) {
        auto& descriptorSet = a_ExecutionState.pipelineState.at(bindingPoint).pushDescriptorSet;
        for (uint16_t i = 0; i < writeCount; ++i) {
            const auto& write = writes.at(i);
            descriptorSet.push_back({ write, write.dstBinding });
        }
    }
    const Pipeline::Layout::Handle layout;
    const uint8_t  bindingPoint;
    const uint16_t writeCount;
    std::array<Descriptor::Set::WriteOperation, 256> writes;
};

void BindDescriptorSet(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const Pipeline::BindingPoint&   a_BindingPoint,
    const Pipeline::Layout::Handle& a_Layout,
    const Descriptor::Set::Handle&  a_DescriptorSet,
    const uint32_t                  a_DynamicOffset)
{
    a_CommandBuffer->PushCommand<BindDescriptorSetCommand>(
        a_BindingPoint,
        a_Layout,
        a_DescriptorSet,
        a_DynamicOffset);
}

void PushDescriptorSet(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const Pipeline::BindingPoint&   a_BindingPoint,
    const Pipeline::Layout::Handle& a_Layout,
    const std::vector<Descriptor::Set::WriteOperation>& a_Writes)
{
    a_CommandBuffer->PushCommand<PushDescriptorSetCommand>(
        a_BindingPoint,
        a_Layout,
        a_Writes.size(), a_Writes.data());
}
}

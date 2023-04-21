////////////////////////////////////////////////////////////////////////////////
//  Buffer commands
////////////////////////////////////////////////////////////////////////////////

#include <GL/Common/Assert.hpp>
#include <GL/CommandBuffer.hpp>
#include <GL/ExecutionState.hpp>
#include <GL/Pipeline.hpp>

namespace OCRA::Command{
struct BindDescriptorSetCommand : CommandI {
    BindDescriptorSetCommand(
        const PipelineBindingPoint&     a_BindingPoint,
        const Descriptor::Set::Handle&  a_DescriptorSet,
        const uint32_t                  a_DynamicOffset)
        : descriptorSet(a_DescriptorSet)
        , bindingPoint(uint8_t(a_BindingPoint))
        , dynamicOffset(a_DynamicOffset)
    {}
    virtual void operator()(Buffer::ExecutionState& a_ExecutionState) {
        auto& pipelineState = a_ExecutionState.pipelineState.at(bindingPoint);
        pipelineState.descriptorSet = descriptorSet;
        pipelineState.descriptorDynamicOffset = dynamicOffset;
    }
    const Descriptor::Set::Handle  descriptorSet;
    const uint8_t  bindingPoint;
    const uint32_t dynamicOffset;
};

struct PushDescriptorSetCommand : CommandI {
    PushDescriptorSetCommand(
        const PipelineBindingPoint& a_BindingPoint,
        const size_t& a_WriteCount,
        const DescriptorSetWrite* a_Writes)
        : bindingPoint(uint8_t(a_BindingPoint))
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
    const uint8_t  bindingPoint;
    const uint16_t writeCount;
    std::array<DescriptorSetWrite, 256> writes;
};

void BindDescriptorSet(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const Pipeline::Handle&         a_Pipeline,
    const Descriptor::Set::Handle&  a_DescriptorSet,
    const uint32_t                  a_DynamicOffset)
{
    a_CommandBuffer->PushCommand<BindDescriptorSetCommand>(
        a_Pipeline->bindingPoint,
        a_DescriptorSet,
        a_DynamicOffset);
}

void PushDescriptorSet(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const Pipeline::Handle&         a_Pipeline,
    const std::vector<DescriptorSetWrite>& a_Writes)
{
    a_CommandBuffer->PushCommand<PushDescriptorSetCommand>(
        a_Pipeline->bindingPoint,
        a_Writes.size(), a_Writes.data());
}
}

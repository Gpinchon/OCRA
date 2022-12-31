#include <OCRA/Descriptor/Set.hpp>

#include <GL/Descriptor/Set.hpp>
#include <GL/Command/Buffer.hpp>
#include <GL/Command/ExecutionState.hpp>
#include <GL/Pipeline/Pipeline.hpp>
#include <GL/Pipeline/Layout.hpp>

#include <GL/glew.h>

namespace OCRA::Descriptor::Set {
void Update(
    const Device::Handle& a_Device,
    const std::vector<WriteOperation>&  a_Writes,
    const std::vector<CopyOperation>&   a_Copies)
{
    for (const auto& writeOperation : a_Writes) {
        writeOperation.dstSet->Write(writeOperation);
    }
    for (const auto& copyOperation : a_Copies) {
        copyOperation.dstSet->Copy(copyOperation);
    }
}
}

#include <algorithm>

namespace OCRA::Command
{
void BindDescriptorSets(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const Pipeline::BindingPoint&   a_BindingPoint,
    const Pipeline::Layout::Handle& a_Layout,
    const uint32_t&                 a_FirstSet,
    const std::vector<Descriptor::Set::Handle>&  a_DescriptorSets,
    const std::vector<uint32_t>&                 a_DynamicOffsets)
{
    auto firstDynamicOffset = !a_DynamicOffsets.empty() ? a_Layout->GetDynamicOffset(a_FirstSet, 0) : 0;
    a_CommandBuffer->PushCommand([
        bindingPoint = uint8_t(a_BindingPoint),
        firstSet = a_FirstSet,
        sets = a_DescriptorSets,
        firstDynamicOffset,
        dynamicOffsets = a_DynamicOffsets
    ](Buffer::ExecutionState& a_ExecutionState){
        auto& pipelineState = a_ExecutionState.pipelineState.at(bindingPoint);
        for (uint32_t i = firstSet; i < sets.size(); ++i) {
            auto& descriptorSet = pipelineState.descriptorSets.at(i);
            const auto& bindings = sets.at(i)->bindings;
            descriptorSet.clear();
            for (const auto& binding : sets.at(i)->bindings) {
                descriptorSet.push_back(binding);
            }
        }
        for (uint32_t i = firstDynamicOffset; i < dynamicOffsets.size(); ++i) {
            pipelineState.descriptorDynamicOffsets.at(i) = dynamicOffsets.at(i);
        }
    });
}

void OCRA::Command::PushDescriptorSet(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const Pipeline::BindingPoint&   a_BindingPoint,
    const Pipeline::Layout::Handle& a_Layout,
    const uint32_t&                 a_Set,
    const std::vector<Descriptor::Set::WriteOperation>& a_Writes)
{
    a_CommandBuffer->PushCommand([
        bindingPoint = uint8_t(a_BindingPoint),
        layout = a_Layout,
        set = a_Set,
        writes = a_Writes
    ](Buffer::ExecutionState& a_ExecutionState) {
        auto& descriptorSet = a_ExecutionState.pipelineState.at(bindingPoint).descriptorSets.at(set);
        for (const auto& write : writes) {
            descriptorSet.push_back({ write, write.dstBinding });
        }
    });
}
}


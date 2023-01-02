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

void OCRA::Command::BindDescriptorSet(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const Pipeline::BindingPoint&   a_BindingPoint,
    const Pipeline::Layout::Handle& a_Layout,
    const Descriptor::Set::Handle& a_DescriptorSet,
    const std::vector<uint32_t>&    a_DynamicOffsets)
{
    auto firstDynamicOffset = !a_DynamicOffsets.empty() ? a_Layout->GetDynamicOffset(0) : 0;
    a_CommandBuffer->PushCommand([
        bindingPoint = uint8_t(a_BindingPoint),
        descriptorSet = a_DescriptorSet,
        firstDynamicOffset,
        dynamicOffsets = a_DynamicOffsets
    ](Buffer::ExecutionState& a_ExecutionState){
        auto& pipelineState = a_ExecutionState.pipelineState.at(bindingPoint);
        const auto& bindings = descriptorSet->bindings;
        pipelineState.descriptorSet.clear();
        for (const auto& binding : bindings) {
            pipelineState.descriptorSet.push_back(binding);
        }
        for (uint32_t i = firstDynamicOffset; i < dynamicOffsets.size(); ++i) {
            pipelineState.descriptorDynamicOffset = dynamicOffsets.at(i);
        }
    });
}

void OCRA::Command::PushDescriptorSet(
    const Command::Buffer::Handle&  a_CommandBuffer,
    const Pipeline::BindingPoint&   a_BindingPoint,
    const Pipeline::Layout::Handle& a_Layout,
    const std::vector<Descriptor::Set::WriteOperation>& a_Writes)
{
    a_CommandBuffer->PushCommand([
        bindingPoint = uint8_t(a_BindingPoint),
        layout = a_Layout,
        writes = a_Writes
    ](Buffer::ExecutionState& a_ExecutionState) {
        auto& descriptorSet = a_ExecutionState.pipelineState.at(bindingPoint).descriptorSet;
        for (const auto& write : writes) {
            descriptorSet.push_back({ write, write.dstBinding });
        }
    });
}

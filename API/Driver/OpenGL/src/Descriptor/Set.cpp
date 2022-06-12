#include <Descriptor/Set.hpp>

#include <GL/Descriptor/Set.hpp>
#include <GL/Command/Buffer.hpp>
#include <GL/Command/ExecutionState.hpp>
#include <GL/Pipeline/Pipeline.hpp>
#include <GL/Pipeline/Layout.hpp>

#include <GL/glew.h>

namespace OCRA::Descriptor::Set {
void Update(const Device::Handle& a_Device, const std::vector<WriteOperation> a_Writes, const std::vector<CopyOperation> a_Copies)
{
    for (const auto& writeOperation : a_Writes) {
        writeOperation.dstSet->Write(writeOperation);
    }
    for (const auto& copyOperation : a_Copies) {
        copyOperation.dstSet->Copy(copyOperation);
    }
}
}

namespace OCRA::Command
{
void BindDescriptorSets(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Pipeline::BindingPoint& a_BindingPoint,
    const Pipeline::Layout::Handle& a_PipelineLayout,
    const uint32_t& a_firstSet,
    const std::vector<Descriptor::Set::Handle>  a_DescriptorSets,
    const std::vector<uint32_t>                 a_DynamicOffsets)
{
    Command::Buffer::DescriptorSets descriptorSets;
    descriptorSets.pipelineLayout = a_PipelineLayout;
    descriptorSets.descriptorSets = { a_DescriptorSets.begin() + a_firstSet, a_DescriptorSets.end() };
    descriptorSets.dynamicOffset = a_DynamicOffsets;
    a_CommandBuffer->PushCommand([bindingPoint = size_t(a_BindingPoint), descriptorSets](Buffer::ExecutionState& a_ExecutionState) {
        assert(descriptorSets.pipelineLayout == a_ExecutionState.pipelineState.at(bindingPoint)->layout);
        a_ExecutionState.descriptorSets.at(bindingPoint) = descriptorSets;
        for (auto& set : descriptorSets.descriptorSets) {
            set->Apply();
        }
        //TODO : use descriptorSets.pipelineLayout->info.pushConstants
        a_ExecutionState.pushConstants.Bind();
    });
}
}


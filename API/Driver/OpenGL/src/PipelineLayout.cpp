#include <GL/Device.hpp>
#include <GL/DescriptorSetLayout.hpp>
#include <GL/PipelineLayout.hpp>

#include <GL/glew.h>

namespace OCRA::Device
{
//Pipeline::Layout::Handle CreatePipelineLayout(
//    const Device::Handle&,
//    const CreatePipelineLayoutInfo& a_Info,
//    const AllocationCallback* a_Allocator)
//{
//    return std::make_shared<Pipeline::Layout::Impl>(a_Info);
//}
}

namespace OCRA::Pipeline::Layout
{
//Impl::Impl(const CreatePipelineLayoutInfo& a_Info)
//{
//    uint32_t dynamicOffset = 0;
//    for (uint32_t i = 0; i < a_Info.setLayouts.size(); ++i) {
//        const auto& setLayout = a_Info.setLayouts.at(i);
//        if (setLayout == nullptr) continue;
//        auto bindingsSize = setLayout->bindings.size();
//        descriptorSet.resize(bindingsSize);
//        for (uint32_t j = 0; j < bindingsSize; ++j) {
//            descriptorSet.at(j).type = setLayout->bindings.at(j).type;
//            descriptorSet.at(j).dynamicOffset = dynamicOffset;
//            //descriptorSet.bindings[j].offset = setLayout->bindings.at(j).offset;
//            if (descriptorSet.at(j).type == DescriptorType::StorageBufferDynamic
//            ||  descriptorSet.at(j).type == DescriptorType::UniformBufferDynamic)
//                dynamicOffset += setLayout->bindings.at(j).count;
//        }
//    }
//    pushConstantRanges = a_Info.pushConstants;
//}

//TODO : implement this
bool Impl::IsCompatibleWith(const Handle & a_Layout)
{
    return false;
}
}
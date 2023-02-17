#include <GL/Pipeline/Layout.hpp>
#include <GL/Device.hpp>
#include <GL/Descriptor/SetLayout.hpp>

#include <GL/glew.h>

namespace OCRA::Pipeline::Layout
{
Impl::Impl(const Info& a_Info)
{
    uint32_t dynamicOffset = 0;
    for (uint32_t i = 0; i < a_Info.setLayouts.size(); ++i) {
        const auto& setLayout = a_Info.setLayouts.at(i);
        if (setLayout == nullptr) continue;
        auto bindingsSize = setLayout->bindings.size();
        descriptorSet.resize(bindingsSize);
        for (uint32_t j = 0; j < bindingsSize; ++j) {
            descriptorSet.at(j).type = setLayout->bindings.at(j).type;
            descriptorSet.at(j).dynamicOffset = dynamicOffset;
            //descriptorSet.bindings[j].offset = setLayout->bindings.at(j).offset;
            if (descriptorSet.at(j).type == Descriptor::Type::StorageBufferDynamic
            ||  descriptorSet.at(j).type == Descriptor::Type::UniformBufferDynamic)
                dynamicOffset += setLayout->bindings.at(j).count;
        }
    }
    pushConstantRanges = a_Info.pushConstants;
}

//TODO : implement this
bool Impl::IsCompatibleWith(const Handle & a_Layout)
{
    return false;
}

Handle Create(const Device::Handle&, const Info& a_Info, const AllocationCallback* a_Allocator)
{
    return Handle(new Impl(a_Info));
}
}
#include <GL/Descriptor/SetLayout.hpp>

OCRA_DECLARE_HANDLE(OCRA::Descriptor::SetLayout);

namespace OCRA::Device
{
Descriptor::SetLayout::Handle CreateDescriptorSetLayout(
    const Device::Handle& a_Device,
    const CreateDescriptorSetLayoutInfo& a_Info,
    const AllocationCallback* a_Allocator)
{
    return std::make_shared<Descriptor::SetLayout::Impl>(a_Device, a_Info);
}
}
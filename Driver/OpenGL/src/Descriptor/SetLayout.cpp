#include <GL/Descriptor/SetLayout.hpp>

OCRA_DECLARE_HANDLE(OCRA::Descriptor::SetLayout);

namespace OCRA::Descriptor::SetLayout
{
Handle Create(const Device::Handle& a_Device, const Info& a_Info, const AllocationCallback* a_Allocator)
{
	return Handle(new Impl(a_Device, a_Info));
}
}
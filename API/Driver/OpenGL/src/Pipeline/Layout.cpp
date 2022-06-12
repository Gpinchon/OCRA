#include <GL/Pipeline/Layout.hpp>
#include <GL/Device.hpp>

#include <GL/glew.h>

#include <algorithm>
#include <cassert>

namespace OCRA::Pipeline::Layout
{
Impl::Impl(const Device::Handle& a_Device, const Info& a_Info)
    : device(a_Device)
    , info(a_Info)
{}

//TODO : implement this
bool Impl::IsCompatibleWith(const Handle & a_Layout)
{
    return false;
}

Handle Create(const Device::Handle& a_Device, const Info& a_Info, const AllocationCallback* a_Allocator)
{
    return Handle(new Impl(a_Device, a_Info));
}
}
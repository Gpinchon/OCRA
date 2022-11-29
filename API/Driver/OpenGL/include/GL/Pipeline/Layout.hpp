#pragma once

#include <Pipeline/Layout.hpp>


OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Pipeline::Layout
{
struct Impl
{
    Impl(const Device::Handle& a_Device, const Info& a_Info);
    bool IsCompatibleWith(const Handle& a_Layout);
    const Device::WeakHandle device;
    const Info    info;
};
}

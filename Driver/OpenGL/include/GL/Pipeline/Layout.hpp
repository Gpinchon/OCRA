#pragma once

#include <Pipeline/Layout.hpp>

namespace OCRA::Pipeline::Layout
{
struct Impl
{
    Impl(const Device::Handle& a_Device, const Info& a_Info);
    const Info    info;
    const uint8_t pushConstantSize;
};
}

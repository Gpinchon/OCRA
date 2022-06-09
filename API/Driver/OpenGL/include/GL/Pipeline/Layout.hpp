#pragma once

#include <Pipeline/Layout.hpp>

#include <GL/WeakHandle.hpp>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Pipeline::Layout
{
struct Impl
{
    Impl(const Device::Handle& a_Device, const Info& a_Info);
    ~Impl();
    void Apply();
    const Device::WeakHandle device;
    const Info    info;
    const uint8_t pushConstantSize;
    const uint32_t pushConstantHandle;
};
}

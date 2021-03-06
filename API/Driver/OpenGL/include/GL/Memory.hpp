#pragma once

#include <Handle.hpp>
#include <Memory.hpp>

#include <GL/WeakHandle.hpp>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Memory);

namespace OCRA::Memory
{
struct Impl
{
    Impl(const Device::Handle& a_Device, const Info& a_Info);
    ~Impl();
    const Device::WeakHandle device;
    const Info  info;
    uint32_t    handle{ 0 };
    uint32_t    mapFlags{ 0 };
};
}

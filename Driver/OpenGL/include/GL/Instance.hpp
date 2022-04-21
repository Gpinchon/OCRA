#pragma once

#include <Handle.hpp>
#include <Instance.hpp>

#include <functional>

OCRA_DECLARE_HANDLE(OCRA::Instance);

namespace OCRA::Instance
{

struct Impl
{
    Impl(const Info& a_Info);
    ~Impl();
    const Info info;
    void* displayHandle;
    std::vector<PhysicalDevice::Handle> physicalDevices;
    static constexpr auto type{ "OpenGL" };
};
}
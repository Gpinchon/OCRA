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
    const Info info;
    std::vector<PhysicalDevice::Handle> physicalDevices;
    static constexpr auto type{ "OpenGL" };
};
}
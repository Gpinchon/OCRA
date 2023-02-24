#pragma once

#include <OCRA/Handle.hpp>
#include <OCRA/Instance.hpp>

#include <functional>

OCRA_DECLARE_HANDLE(OCRA::PhysicalDevice);

namespace OCRA::Instance
{
struct Impl
{
    Impl();
    const uint32_t  id;
    std::vector<PhysicalDevice::Handle> physicalDevices;
    static constexpr auto type{ "OpenGL" };
};
}

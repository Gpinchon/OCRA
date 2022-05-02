#pragma once

#include <Handle.hpp>
#include <Instance.hpp>
#include <Surface.hpp>

#include <functional>

OCRA_DECLARE_HANDLE(OCRA::PhysicalDevice);

namespace OCRA::Instance
{
struct Impl
{
    Impl(const Info& a_Info);
    const uint32_t  id;
    const Info      info;
    std::vector<PhysicalDevice::Handle> physicalDevices;
    static constexpr auto type{ "OpenGL" };
};
}

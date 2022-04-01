#pragma once

#include <Handle.hpp>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::PhysicalDevice);

namespace OCRA::Device
{
PhysicalDevice::Handle GetPhysicalDevice(const Handle& a_Device);
}
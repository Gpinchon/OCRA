#pragma once

#include <OCRA/Handles.hpp>
#include <OCRA/SwapChain/Core.hpp>

#include <vector>

namespace OCRA::PhysicalDevice {
/**
 * @return the list of supported surface formats
 */
std::vector<SurfaceFormat> GetSurfaceFormats(
    const PhysicalDevice::Handle& a_PhysicalDevice,
    const Surface::Handle& a_Surface);
}

#pragma once

#include <OCRA/Handles.hpp>

#include <vector>

namespace OCRA::Instance {
struct Impl {
    Impl();
    const uint32_t id;
    std::vector<PhysicalDevice::Handle> physicalDevices;
    static constexpr auto type { "OpenGL" };
};
}

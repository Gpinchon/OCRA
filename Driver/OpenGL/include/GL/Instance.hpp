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
    const uint32_t  id;
    const Info      info;
    std::vector<PhysicalDevice::Handle> physicalDevices;
    static constexpr auto type{ "OpenGL" };
#ifdef _WIN32
    //required for OGL context creation on Windows
    void* dummyWindow;
    void* dummyDevice;
#endif //_WIN32
};
}
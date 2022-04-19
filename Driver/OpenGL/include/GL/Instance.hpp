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
    inline void PushCommand(const std::function<void()>& a_Command, const bool a_Synchronous) {
        glThread.PushCommand(a_Command, a_Synchronous);
    }
    void* displayHandle;
    const Info info;
    std::vector<PhysicalDevice::Handle> physicalDevices;
    static constexpr auto type{ "OpenGL" };
};
void MakeCurrent(const Handle& a_Instance, const void* a_HDC);
}
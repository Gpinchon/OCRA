#pragma once

#include <Handle.hpp>

#include <string>

OCRA_DECLARE_HANDLE(OCRA::Instance);
OCRA_DECLARE_HANDLE(OCRA::Instance);

namespace OCRA::Surface
{
struct Impl
{
    Impl(const std::string& a_Type) : type(a_Type) {}
    const std::string type;
    void* nativeDisplay;
    void* nativeWindow;
};
}

#ifdef _WIN32
namespace OCRA::Surface::Win32
{
struct Impl : Surface::Impl
{
    Impl(const Instance::Handle& a_Instance, const Info& a_Info);
    const Info info;
    const Instance::WeakHandle instance;
};
}
#endif //_WIN32
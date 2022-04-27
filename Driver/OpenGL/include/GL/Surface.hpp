#pragma once

#include <Handle.hpp>
#include <Surface.hpp>

#include <GL/WeakHandle.hpp>

#include <string>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Instance);
OCRA_DECLARE_HANDLE(OCRA::Instance);

namespace OCRA::Surface
{
struct Impl
{
    Impl(const Instance::Handle& a_Instance, const void* a_NativeWindow, const std::string& a_Type)
        : type(a_Type)
        , instance(a_Instance)
        , nativeWindow(a_NativeWindow)
    {}
    const std::string type;
    const Instance::WeakHandle instance;
    const void* nativeWindow;
};
}

#ifdef _WIN32
namespace OCRA::Surface::Win32
{
struct Impl : Surface::Impl
{
    Impl(const Instance::Handle& a_Instance, const Info& a_Info);
    ~Impl();
    const void* hdc;
};
}
#endif //_WIN32
#pragma once

#include <OCRA/Handles.hpp>
#include <OCRA/SwapChain/Handles.hpp>

#include <string>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Instance);

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

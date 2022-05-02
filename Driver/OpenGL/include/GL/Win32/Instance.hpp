#pragma once
#include <Instance.hpp>
#include <GL/Instance.hpp>

namespace OCRA::Instance::Win32
{
struct Impl : Instance::Impl
{
	Impl(const Info& a_Info);
    ~Impl();
    //required for OGL context creation on Windows
    void* hwnd;
};
Handle Create(const Info& a_Info);
}

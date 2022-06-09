/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:27:22
*/
#pragma once

#include <Handle.hpp>

OCRA_DECLARE_HANDLE(OCRA::Instance);
OCRA_DECLARE_HANDLE(OCRA::Surface);

#ifdef _WIN32
namespace OCRA::Surface::Win32 {
struct Info {
    void* hinstance;
    void* hwnd;
};
Handle Create(const Instance::Handle& a_Instance, const Info& a_Info);
}
#endif 

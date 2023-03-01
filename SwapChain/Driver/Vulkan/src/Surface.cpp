#include <Surface.hpp>

#include <VK/Instance.hpp>

#include <OCRA/Core.hpp>
#include <OCRA/SwapChain.hpp>

#ifdef _WIN32
  #include <Windows.h>
  #include <vulkan/vulkan_win32.h>
#endif //_WIN32

namespace OCRA::Instance
{
Surface::Handle CreateSurface(
    const Instance::Handle& a_Instance,
    const CreateSurfaceInfo& a_Info)
{
    VkSurfaceKHR surface = nullptr;
#ifdef _WIN32
    VkWin32SurfaceCreateInfoKHR vkInfo{ VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
    vkInfo.hinstance = HINSTANCE(a_Info.hinstance);
    vkInfo.hwnd = HWND(a_Info.hwnd);
    vkCreateWin32SurfaceKHR(*a_Instance, &vkInfo, nullptr, &surface);
#endif //_WIN32
    return std::make_shared<Surface::Impl>(*a_Instance, surface);
}
}

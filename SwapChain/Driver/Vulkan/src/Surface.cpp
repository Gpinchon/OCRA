#include <VK/SwapChain/Surface.hpp>

#include <VK/Enums.hpp>
#include <VK/Instance.hpp>
#include <VK/PhysicalDevice.hpp>

#include <OCRA/Core.hpp>
#include <OCRA/SwapChain.hpp>

#ifdef _WIN32
#include <Windows.h>
#include <vulkan/vulkan_win32.h>
#endif //_WIN32

namespace OCRA::Instance {
Surface::Handle CreateSurface(
    const Instance::Handle& a_Instance,
    const CreateSurfaceInfo& a_Info)
{
    VkSurfaceKHR surface = nullptr;
#ifdef _WIN32
    VkWin32SurfaceCreateInfoKHR vkInfo { VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR };
    vkInfo.hinstance = HINSTANCE(a_Info.hinstance);
    vkInfo.hwnd      = HWND(a_Info.hwnd);
    vkCreateWin32SurfaceKHR(**a_Instance, &vkInfo, nullptr, &surface);
#endif //_WIN32
    return std::make_shared<Surface::Impl>(*a_Instance, surface);
}
}

namespace OCRA::PhysicalDevice {
std::vector<SurfaceFormat> GetSurfaceFormats(
    const PhysicalDevice::Handle& a_PhysicalDevice,
    const Surface::Handle& a_Surface)
{
    auto& physicalDevice = *a_PhysicalDevice;
    auto& surface        = *a_Surface;

    auto vkFormats = physicalDevice.getSurfaceFormatsKHR(*surface);
    std::vector<SurfaceFormat> formats(vkFormats.size());
    for (auto i = 0u; i < vkFormats.size(); ++i) {
        auto& format      = formats.at(i);
        auto& vkFormat    = vkFormats.at(i);
        format.colorSpace = ConvertFromVk(vkFormat.colorSpace);
        format.format     = ConvertFromVk(vkFormat.format);
    }
    return formats;
}
}

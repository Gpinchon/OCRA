#include <Instance.hpp>

#include <GL/Instance.hpp>
#include <GL/PhysicalDevice.hpp>
//#include <GL/eglew.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>

#include <stdexcept>

namespace OCRA::Instance
{
Impl::Impl(const Info& a_Info) : info(a_Info)
{
    //if (eglewInit() != GLEW_OK) throw std::runtime_error("Cound not initialize EGLEW");
    constexpr EGLint maxDevices = 16;
    EGLDeviceEXT    devices[maxDevices];
    EGLint          devicesNum;
    eglQueryDevicesEXT(maxDevices, devices, &devicesNum);
    if (eglGetError() != EGL_SUCCESS) throw std::runtime_error("Could not query devices");
    for (auto deviceIndex = 0; deviceIndex < devicesNum; ++deviceIndex) {
        physicalDevices.push_back(PhysicalDevice::Create(devices[deviceIndex]));
    }
    
}
Handle Create(
	const Info& a_Info,
	const AllocationCallback* a_Allocator)
{
	return Handle(new Impl(a_Info));
}
const std::string& GetType(const Handle& a_Instance)
{
	return Impl::type;
}
const Info& GetInfo(const Handle& a_Instance)
{
	return a_Instance->info;
}
const std::vector<PhysicalDevice::Handle>& EnumeratePhysicalDevices(const Instance::Handle& a_Instance)
{
	return a_Instance->physicalDevices;
}
}
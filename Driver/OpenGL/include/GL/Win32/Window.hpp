#include <string>

namespace OCRA::Window::Win32
{
void* Create(const std::string& a_ClassName, const std::string& a_Name);
void SetDefaultPixelFormat(const void* a_DeviceHandle);
}
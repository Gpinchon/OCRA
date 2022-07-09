#include <Common.hpp>
#include <Instance.hpp>
#include <PhysicalDevice.hpp>
#include <Device.hpp>
#include <Queue/Queue.hpp>
#include <Command/Pool.hpp>
#include <Memory.hpp>

#include <iostream>
#include <vector>
#include <stdexcept>
#include <windows.h>

LRESULT CALLBACK TestWndproc(
    HWND hwnd,        // handle to window
    UINT uMsg,        // message identifier
    WPARAM wParam,    // first message parameter
    LPARAM lParam)    // second message parameter
{
    const auto window = (OCRA::Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    if (window == nullptr) return DefWindowProc(hwnd, uMsg, wParam, lParam);
    switch (uMsg)
    {
    case WM_PAINT :
        if (window->OnPaint) window->OnPaint(*window);
        break;
    case WM_SIZE:
        switch (wParam) {
        case SIZE_MAXIMIZED:
            if (window->OnMaximize) window->OnMaximize(*window, LOWORD(lParam), HIWORD(lParam));
            break;
        case SIZE_MINIMIZED:
            if (window->OnMinimize) window->OnMinimize(*window, LOWORD(lParam), HIWORD(lParam));
            break;
        case SIZE_RESTORED:
            if (window->OnRestore) window->OnRestore(*window, LOWORD(lParam), HIWORD(lParam));
            break;
        default:
            if (window->OnResize) window->OnResize(*window, LOWORD(lParam), HIWORD(lParam));
        }
        break;
    case WM_CLOSE:
        if (window->OnClose) window->OnClose(*window);
        break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

namespace OCRA
{

struct WindowClass : WNDCLASSEX
{
    WindowClass()
    {
        std::memset(this, 0, sizeof(WindowClass));
        cbSize = sizeof(WindowClass);
        style = CS_HREDRAW | CS_VREDRAW;
        lpfnWndProc = TestWndproc;
        hInstance = GetModuleHandle(0);
        lpszClassName = "TestWindow";
        if (!RegisterClassEx(this)) {
            std::cerr << "Error in Function = " << __FUNCTION__ << " at line = " << __LINE__ << ", with error code = " << GetLastError() << std::endl;
            throw std::runtime_error("Could not register window class");
        }
    }
    ~WindowClass()
    {
        UnregisterClass(lpszClassName, hInstance);
    }
};

static inline auto& GetWindowClass()
{
    static WindowClass wndclass;
    return wndclass;
}

Window::Window(const std::string& name, const uint32_t a_Width, const uint32_t a_Height)
{
    const auto& wndclass = GetWindowClass();
    const auto hwnd = CreateWindowEx(
        0,
        wndclass.lpszClassName,
        name.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, a_Width, a_Height,
        nullptr, nullptr, wndclass.hInstance, nullptr);
    MSG msg{ 0 };
    while (PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE)) {
        DispatchMessage(&msg);
        if (msg.message == WM_CREATE) break;
    }
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);
    nativeHandle = hwnd;
}

Window::~Window()
{
    DestroyWindow(HWND(nativeHandle));
}

void Window::PushEvents()
{
    MSG msg{ 0 };
    while (PeekMessage(&msg, HWND(nativeHandle), 0, 0, PM_REMOVE))
        DispatchMessage(&msg);
}

void Window::Show()
{
    ShowWindow(HWND(nativeHandle), SW_SHOWDEFAULT);
    UpdateWindow(HWND(nativeHandle));
}

void Window::Update()
{
    UpdateWindow(HWND(nativeHandle));
}

//Create OCRA Instance
Instance::Handle CreateInstance(const std::string& a_Name)
{
    Instance::Handle instance;
    Instance::Info instanceInfo;
    instanceInfo.applicationInfo.name = a_Name;
    instanceInfo.applicationInfo.applicationVersion = 1;
    instance = Instance::Create(instanceInfo);
    std::cout << "==== Instance ====\n";
    std::cout << "  Type           : " << Instance::GetType(instance) << "\n";
    std::cout << "  App Name       : " << Instance::GetInfo(instance).applicationInfo.name << "\n";
    std::cout << "  App Version    : " << Instance::GetInfo(instance).applicationInfo.applicationVersion << "\n";
    std::cout << "  Engine Name    : " << Instance::GetInfo(instance).applicationInfo.engineName << "\n";
    std::cout << "  Engine Version : " << Instance::GetInfo(instance).applicationInfo.engineVersion << "\n";
    std::cout << "==================\n";
    std::cout << "\n";
    return instance;
}

std::vector<Queue::Info> GetQueueInfos(const PhysicalDevice::Handle& a_PhysicalDevice)
{
    std::vector<Queue::Info> queueInfos;
    auto& queueFamilies = PhysicalDevice::GetQueueFamilyProperties(a_PhysicalDevice);
    uint32_t familyIndex = 0;
    for (auto& queueFamily : queueFamilies)
    {
        Queue::Info queueInfo;
        queueInfo.queueCount = queueFamily.queueCount;
        queueInfo.queueFamilyIndex = familyIndex;
        queueInfo.queuePriorities.resize(queueFamily.queueCount, 1.f);
        queueInfos.push_back(queueInfo);
        ++familyIndex;
    }
    return queueInfos;
}

void PrintQueueInfos(const PhysicalDevice::Handle& a_PhysicalDevice)
{
    const auto& queueFamilies = PhysicalDevice::GetQueueFamilyProperties(a_PhysicalDevice);
    uint32_t familyIndex = 0;
    std::cout << "==== Queue Families ====\n";
    for (auto& queueFamily : queueFamilies)
    {
        std::cout << " ==================\n";
        std::cout << "  Index         : " << familyIndex << "\n";
        std::cout << "  Count         : " << queueFamily.queueCount << "\n";
        std::cout << " == Capabilities ==\n";
        std::cout << "  Graphics      : " << ((queueFamily.queueFlags & PhysicalDevice::QueueFlagsBits::Graphics) != 0) << "\n";
        std::cout << "  Compute       : " << ((queueFamily.queueFlags & PhysicalDevice::QueueFlagsBits::Compute) != 0) << "\n";
        std::cout << "  Protected     : " << ((queueFamily.queueFlags & PhysicalDevice::QueueFlagsBits::Protected) != 0) << "\n";
        std::cout << "  SparseBinding : " << ((queueFamily.queueFlags & PhysicalDevice::QueueFlagsBits::SparseBinding) != 0) << "\n";
        std::cout << "  Transfer      : " << ((queueFamily.queueFlags & PhysicalDevice::QueueFlagsBits::Transfer) != 0) << "\n";
        std::cout << " ==================\n";
        ++familyIndex;
    }
    std::cout << "========================\n";
    std::cout << "\n";
}

Device::Handle CreateDevice(const PhysicalDevice::Handle& a_PhysicalDevice)
{
    Device::Info deviceInfo;
    deviceInfo.queueInfos = GetQueueInfos(a_PhysicalDevice);
    return Device::Create(a_PhysicalDevice, deviceInfo);
}

uint32_t FindQueueFamily(const PhysicalDevice::Handle& a_PhysicalDevice, const PhysicalDevice::QueueFlags& a_QueueProperties)
{
    auto& queueProperties = PhysicalDevice::GetQueueFamilyProperties(a_PhysicalDevice);
    for (auto familyIndex = 0u; familyIndex < queueProperties.size(); ++familyIndex) {
        if (queueProperties.at(familyIndex).queueFlags == a_QueueProperties)
            return familyIndex;
    }
    return std::numeric_limits<uint32_t>::infinity();
}

Command::Pool::Handle CreateCommandPool(const Device::Handle& a_Device, const uint32_t& a_QueueFamily)
{
    Command::Pool::Info commandPoolInfo;
    commandPoolInfo.queueFamilyIndex = a_QueueFamily;
    return Command::Pool::Create(a_Device, commandPoolInfo);
}

Command::Buffer::Handle CreateCommandBuffer(const Device::Handle& a_Device, const Command::Pool::Handle& a_CommandPool, const Command::Pool::AllocateInfo::Level& a_Level)
{
    Command::Pool::AllocateInfo commandBufferAllocateInfo;
    commandBufferAllocateInfo.pool = a_CommandPool;
    commandBufferAllocateInfo.count = 1;
    commandBufferAllocateInfo.level = a_Level;
    return Command::Pool::AllocateBuffer(a_Device, commandBufferAllocateInfo).front();
}

void SubmitCommandBuffer(const Queue::Handle& a_Queue, const Command::Buffer::Handle& a_CommandBuffer)
{
    Queue::SubmitInfo submitInfo;
    submitInfo.commandBuffers.push_back(a_CommandBuffer);
    Queue::Submit(a_Queue, { submitInfo });
}
/*

*/
uint32_t FindProperMemoryType(const PhysicalDevice::Handle& a_PhysicalDevice, const PhysicalDevice::MemoryPropertyFlags& a_MemoryProperties)
{
    auto& memoryProperties = PhysicalDevice::GetMemoryProperties(a_PhysicalDevice);
    for (auto memoryTypeIndex = 0u; memoryTypeIndex < memoryProperties.memoryTypes.size(); ++memoryTypeIndex) {
        if (memoryProperties.memoryTypes.at(memoryTypeIndex).propertyFlags == a_MemoryProperties)
            return memoryTypeIndex;
    }
    //Couldn't find optimal memory type, take any fitting type
    for (auto memoryTypeIndex = 0u; memoryTypeIndex < memoryProperties.memoryTypes.size(); ++memoryTypeIndex) {
        if ((memoryProperties.memoryTypes.at(memoryTypeIndex).propertyFlags & a_MemoryProperties) != 0)
            return memoryTypeIndex;
    }
    throw std::runtime_error("Could not find matching memory type");
    return (std::numeric_limits<uint32_t>::max)();
}
Memory::Handle AllocateMemory(const PhysicalDevice::Handle& a_PhysicalDevice, const Device::Handle& a_Device, const uint64_t& a_Size, const PhysicalDevice::MemoryPropertyFlags& a_MemoryProperties)
{
    Memory::Info memoryInfo;
    memoryInfo.memoryTypeIndex = FindProperMemoryType(a_PhysicalDevice, a_MemoryProperties);
    memoryInfo.size = a_Size;
    return Memory::Allocate(a_Device, memoryInfo);
}
}
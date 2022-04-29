#include <Instance.hpp>
#include <PhysicalDevice.hpp>
#include <Device.hpp>
#include <Queue/Queue.hpp>
#include <Command/Pool.hpp>
#include <Surface.hpp>
#include <SwapChain.hpp>
#include <Memory.hpp>

#include <iostream>
#include <vector>
#include <windows.h>

namespace OCRA
{
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

Surface::Handle CreateSurface(const Instance::Handle& a_Instance, const void* a_HINSTANCE, const void* a_HWND)
{
    Surface::Win32::Info info{};
    info.hinstance = HINSTANCE(a_HINSTANCE);
    info.hwnd = HWND(a_HWND);
    return Surface::Win32::Create(a_Instance, info);
}

SwapChain::Handle CreateSwapChain(const Device::Handle& a_Device, const Surface::Handle& a_Surface, const SwapChain::Handle& a_OldSwapChain, const uint32_t& a_Width, const uint32_t& a_Height, const uint32_t& a_ImageNbr)
{
    SwapChain::Info info{};
    info.oldSwapchain = a_OldSwapChain;
    info.presentMode = SwapChain::PresentMode::Immediate;
    info.imageFormat = Image::Format::Uint8_Normalized_RGBA;
    info.minImageCount = a_ImageNbr;
    info.surface = a_Surface;
    info.imageExtent.width  = a_Width;
    info.imageExtent.height = a_Height;
    return SwapChain::Create(a_Device, info);
}
uint32_t FindProperMemoryType(const PhysicalDevice::Handle& a_PhysicalDevice, const PhysicalDevice::MemoryPropertyFlags& a_MemoryProperties)
{
    auto& memoryProperties = PhysicalDevice::GetMemoryProperties(a_PhysicalDevice);
    for (auto memoryTypeIndex = 0u; memoryTypeIndex < memoryProperties.memoryTypes.size(); ++memoryTypeIndex) {
        if (memoryProperties.memoryTypes.at(memoryTypeIndex).propertyFlags == a_MemoryProperties)
            return memoryTypeIndex;
    }
    return std::numeric_limits<uint32_t>::infinity();
}
Memory::Handle AllocateMemory(const PhysicalDevice::Handle& a_PhysicalDevice, const Device::Handle& a_Device, const uint64_t& a_Size, const PhysicalDevice::MemoryPropertyFlags& a_MemoryProperties)
{
    Memory::Info memoryInfo;
    memoryInfo.memoryTypeIndex = FindProperMemoryType(a_PhysicalDevice, a_MemoryProperties);
    memoryInfo.size = a_Size;
    return Memory::Allocate(a_Device, memoryInfo);
}
}
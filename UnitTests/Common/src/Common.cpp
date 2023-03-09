#include <Common.hpp>
#include <OCRA/OCRA.hpp>

#include <iostream>
#include <vector>
#include <stdexcept>

namespace OCRA
{
//Create OCRA Instance
Instance::Handle CreateInstance(const std::string& a_Name)
{
    Instance::Handle instance;
    CreateInstanceInfo instanceInfo;
    instanceInfo.applicationInfo.name = a_Name;
    instanceInfo.applicationInfo.applicationVersion = 1;
    instance = CreateInstance(instanceInfo);
    std::cout << "==== Instance ====\n";
    std::cout << "  Type           : " << Instance::GetType(instance) << "\n";
    std::cout << "  App Name       : " << instanceInfo.applicationInfo.name << "\n";
    std::cout << "  App Version    : " << instanceInfo.applicationInfo.applicationVersion << "\n";
    std::cout << "  Engine Name    : " << instanceInfo.applicationInfo.engineName << "\n";
    std::cout << "  Engine Version : " << instanceInfo.applicationInfo.engineVersion << "\n";
    std::cout << "==================\n";
    std::cout << "\n";
    return instance;
}

std::vector<QueueInfo> GetQueueInfos(const PhysicalDevice::Handle& a_PhysicalDevice)
{
    std::vector<QueueInfo> queueInfos;
    auto& queueFamilies = PhysicalDevice::GetQueueFamilyProperties(a_PhysicalDevice);
    uint32_t familyIndex = 0;
    for (auto& queueFamily : queueFamilies)
    {
        QueueInfo queueInfo;
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
        std::cout << "  Graphics      : " << ((queueFamily.queueFlags & QueueFlagBits::Graphics) != 0) << "\n";
        std::cout << "  Compute       : " << ((queueFamily.queueFlags & QueueFlagBits::Compute) != 0) << "\n";
        std::cout << "  Protected     : " << ((queueFamily.queueFlags & QueueFlagBits::Protected) != 0) << "\n";
        std::cout << "  SparseBinding : " << ((queueFamily.queueFlags & QueueFlagBits::SparseBinding) != 0) << "\n";
        std::cout << "  Transfer      : " << ((queueFamily.queueFlags & QueueFlagBits::Transfer) != 0) << "\n";
        std::cout << " ==================\n";
        ++familyIndex;
    }
    std::cout << "========================\n";
    std::cout << "\n";
}

Device::Handle CreateDevice(const PhysicalDevice::Handle& a_PhysicalDevice)
{
    CreateDeviceInfo deviceInfo;
    deviceInfo.queueInfos = GetQueueInfos(a_PhysicalDevice);
    return CreateDevice(a_PhysicalDevice, deviceInfo);
}

Command::Pool::Handle CreateCommandPool(const Device::Handle& a_Device, const uint32_t& a_QueueFamily)
{
    CreateCommandPoolInfo commandPoolInfo;
    commandPoolInfo.flags = CreateCommandPoolFlagBits::Reset;
    commandPoolInfo.queueFamilyIndex = a_QueueFamily;
    return CreateCommandPool(a_Device, commandPoolInfo);
}

Command::Buffer::Handle CreateCommandBuffer(const Command::Pool::Handle& a_CommandPool, const CommandBufferLevel& a_Level)
{
    AllocateCommandBufferInfo commandBufferAllocateInfo;
    commandBufferAllocateInfo.count = 1;
    commandBufferAllocateInfo.level = a_Level;
    return Command::Pool::AllocateCommandBuffer(a_CommandPool, commandBufferAllocateInfo).front();
}

//Memory::Handle AllocateMemory(const PhysicalDevice::Handle& a_PhysicalDevice, const Device::Handle& a_Device, const uint64_t& a_Size, const MemoryPropertyFlags& a_MemoryProperties)
//{
//    AllocateMemoryInfo memoryInfo;
//    memoryInfo.memoryTypeIndex = FindMemoryType(a_PhysicalDevice, a_MemoryProperties);
//    memoryInfo.size = a_Size;
//    return AllocateMemory(a_Device, memoryInfo);
//}

Descriptor::Pool::Handle CreateDescriptorPool(const Device::Handle& a_Device, const size_t& a_MaxSets)
{
    CreateDescriptorPoolInfo poolInfo{};
    poolInfo.maxSets = a_MaxSets;
    poolInfo.sizes = {};
    return CreateDescriptorPool(a_Device, poolInfo);
}
}
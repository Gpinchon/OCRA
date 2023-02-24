#include <Common.hpp>
#include <OCRA/Instance.hpp>
#include <OCRA/PhysicalDevice.hpp>
#include <OCRA/Device.hpp>
#include <OCRA/Queue.hpp>
#include <OCRA/Command/Pool.hpp>
#include <OCRA/Memory.hpp>
#include <OCRA/Descriptor/Pool.hpp>

#include <iostream>
#include <vector>
#include <stdexcept>

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
    std::cout << "  App Name       : " << instanceInfo.applicationInfo.name << "\n";
    std::cout << "  App Version    : " << instanceInfo.applicationInfo.applicationVersion << "\n";
    std::cout << "  Engine Name    : " << instanceInfo.applicationInfo.engineName << "\n";
    std::cout << "  Engine Version : " << instanceInfo.applicationInfo.engineVersion << "\n";
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
        std::cout << "  Graphics      : " << ((queueFamily.queueFlags & Queue::FlagsBits::Graphics) != 0) << "\n";
        std::cout << "  Compute       : " << ((queueFamily.queueFlags & Queue::FlagsBits::Compute) != 0) << "\n";
        std::cout << "  Protected     : " << ((queueFamily.queueFlags & Queue::FlagsBits::Protected) != 0) << "\n";
        std::cout << "  SparseBinding : " << ((queueFamily.queueFlags & Queue::FlagsBits::SparseBinding) != 0) << "\n";
        std::cout << "  Transfer      : " << ((queueFamily.queueFlags & Queue::FlagsBits::Transfer) != 0) << "\n";
        std::cout << " ==================\n";
        ++familyIndex;
    }
    std::cout << "========================\n";
    std::cout << "\n";
}

Device::Handle CreateDevice(const PhysicalDevice::Handle& a_PhysicalDevice)
{
    Device::CreateInfo deviceInfo;
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

Command::Buffer::Handle CreateCommandBuffer(const Command::Pool::Handle& a_CommandPool, const Command::Pool::AllocateInfo::Level& a_Level)
{
    Command::Pool::AllocateInfo commandBufferAllocateInfo;
    commandBufferAllocateInfo.pool = a_CommandPool;
    commandBufferAllocateInfo.count = 1;
    commandBufferAllocateInfo.level = a_Level;
    return Command::Pool::AllocateBuffer(commandBufferAllocateInfo).front();
}

void OCRA::SubmitCommandBuffer(
    const Queue::Handle& a_Queue,
    const Command::Buffer::Handle& a_CommandBuffer,
    const Semaphore::Handle& a_WaitSemaphore,
    const Semaphore::Handle& a_SignalSemaphore)
{
    Queue::SubmitInfo submitInfo;
    submitInfo.commandBuffers.push_back(a_CommandBuffer);
    if (a_WaitSemaphore)   submitInfo.waitSemaphores = { a_WaitSemaphore };
    if (a_SignalSemaphore) submitInfo.signalSemaphores = { a_SignalSemaphore };
    Queue::Submit(a_Queue, { submitInfo });
}

uint32_t FindProperMemoryType(const PhysicalDevice::Handle& a_PhysicalDevice, const Memory::PropertyFlags& a_MemoryProperties)
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

Memory::Handle AllocateMemory(const PhysicalDevice::Handle& a_PhysicalDevice, const Device::Handle& a_Device, const uint64_t& a_Size, const Memory::PropertyFlags& a_MemoryProperties)
{
    Memory::AllocateInfo memoryInfo;
    memoryInfo.memoryTypeIndex = FindProperMemoryType(a_PhysicalDevice, a_MemoryProperties);
    memoryInfo.size = a_Size;
    return  Device::AllocateMemory(a_Device, memoryInfo);
}

Descriptor::Pool::Handle CreateDescriptorPool(const Device::Handle& a_Device, const size_t& a_MaxSets)
{
    Descriptor::Pool::CreateInfo poolInfo{};
    poolInfo.maxSets = a_MaxSets;
    poolInfo.sizes = {};
    return Descriptor::Pool::Create(a_Device, poolInfo);
}
}
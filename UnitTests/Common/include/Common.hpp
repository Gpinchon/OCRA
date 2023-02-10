#pragma once
/**
 * @brief Unit test's common tools
 */

#include <OCRA/OCRA.hpp>

#include <OCRA/Common/Timer.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <functional>
#include <numeric>
#include <algorithm>

OCRA_DECLARE_HANDLE(OCRA::Instance);
OCRA_DECLARE_HANDLE(OCRA::PhysicalDevice);
OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Memory);
OCRA_DECLARE_HANDLE(OCRA::Command::Pool);
OCRA_DECLARE_HANDLE(OCRA::Command::Buffer);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::Pool);

namespace OCRA::PhysicalDevice
{
using QueueFlags = std::bitset<5>;
}

namespace OCRA {
struct VerboseTimer : Timer
{
    VerboseTimer(const std::string& a_Name) : name(a_Name) {}
    ~VerboseTimer() {
        std::cout << "Timer \"" << name << "\" waited : " << Elapsed().count() << " nanoseconds\n";
    }
    const std::string name;
};

class FPSCounter
{
public:
    FPSCounter(const size_t& a_SampleCount = 1000)
        : alpha(1.0 / double(a_SampleCount))
    {}
    void StartFrame() {
        startTime = std::chrono::high_resolution_clock::now();
    }
    void EndFrame() {
        const auto now = std::chrono::high_resolution_clock::now();
        const auto newFrameTime = std::chrono::duration<double, std::milli>(now - startTime).count();
        meanFrameTime = (alpha * newFrameTime) + (1 - alpha) * meanFrameTime;
        fps = 1000.0 / meanFrameTime;
        alpha = 1.0 / fps;
    }
    void Print() const {
        std::cout << "\rFPS : " << fps << std::flush;
    }
    std::chrono::steady_clock::time_point startTime;
    double alpha{ 0.0001 };
    double meanFrameTime{ 0 };
    double fps{ 0 };
};

//Create an instance with app name a_Name
Instance::Handle CreateInstance(const std::string& a_Name);
//Create a device with all available queues
Device::Handle CreateDevice(const PhysicalDevice::Handle& a_PhysicalDevice);
//Get queues from Physical Device
std::vector<Queue::Info> GetQueueInfos(const PhysicalDevice::Handle& a_PhysicalDevice);
//Prints queues informations to the console
void PrintQueueInfos(const PhysicalDevice::Handle& a_PhysicalDevice);
//Find suitable queue for the specified flags
uint32_t FindQueueFamily(const PhysicalDevice::Handle& a_PhysicalDevice, const PhysicalDevice::QueueFlags& a_QueueProperties);
//Create a command pool from the queuefamily
Command::Pool::Handle CreateCommandPool(const Device::Handle& a_Device, const uint32_t& a_QueueFamily);
//Creates a command buffer from the specified command pool
Command::Buffer::Handle CreateCommandBuffer(const Device::Handle& a_Device, const Command::Pool::Handle& a_CommandPool, const Command::Pool::AllocateInfo::Level& a_Level);
//Submits a Command Buffer to the specified Queue
void SubmitCommandBuffer(const Queue::Handle& a_Queue, const Command::Buffer::Handle& a_CommandBuffer);
//Returns the index of a memory type with specified properties
uint32_t FindProperMemoryType(const PhysicalDevice::Handle& a_PhysicalDevice, const PhysicalDevice::MemoryPropertyFlags& a_MemoryProperties);
//allocate GPU memory with the specified properties
Memory::Handle AllocateMemory(const PhysicalDevice::Handle& a_PhysicalDevice, const Device::Handle& a_Device, const uint64_t& a_Size, const PhysicalDevice::MemoryPropertyFlags& a_MemoryProperties);
//creates a Descriptor Pool with the specified max sets
Descriptor::Pool::Handle CreateDescriptorPool(const Device::Handle& a_Device, const size_t& a_MaxSets);

struct TestApp
{
    TestApp(const std::string& a_Name)
        : name(a_Name)
        , instance(CreateInstance(name))
        , physicalDevice(Instance::EnumeratePhysicalDevices(instance).front())
        , device(CreateDevice(physicalDevice))
    {}
    std::string             name;
    Instance::Handle        instance;
    PhysicalDevice::Handle  physicalDevice;
    Device::Handle          device;
};
}

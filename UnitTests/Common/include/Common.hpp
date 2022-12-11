#pragma once
/**
 * @brief Unit test's common tools
 */

#include <OCRA/Handle.hpp>
#include <OCRA/Queue/Queue.hpp>
#include <OCRA/Command/Pool.hpp>
#include <OCRA/PhysicalDevice.hpp>

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
        : frameTimes(a_SampleCount, 0)
    {
    }
    void StartFrame() {
        if (frameNbr < frameTimes.size()) ++frameNbr;
        startTime = std::chrono::high_resolution_clock::now();
    }
    void EndFrame() {
        const auto now = std::chrono::high_resolution_clock::now();
        frameTimes.at(frameTimeIndex) = std::chrono::duration<double, std::milli>(now - startTime).count();
        const auto meanFrameTime = std::accumulate(frameTimes.begin(), frameTimes.begin() + frameNbr, 0.0) / double(frameNbr);
        fps = 1000.0 / meanFrameTime;
        ++frameTimeIndex %= frameTimes.size();
        _UpdateSampleCount();
    }
    void Print() const {
        std::cout << "\rFPS : " << fps << std::flush;
    }
    std::chrono::steady_clock::time_point startTime;
    std::vector<double> frameTimes;
    double fps{ 0 };
    size_t frameTimeIndex{ 0 };
    size_t frameNbr{ 0 };
private:
    void _UpdateSampleCount() {
        size_t sampleCount = 1;
        while (size_t(fps) / sampleCount) {
            sampleCount *= 10;
        }
        sampleCount /= 10;
        sampleCount = std::max(sampleCount, size_t(10));
        sampleCount = sampleCount * (size_t(fps) / sampleCount);
        if (sampleCount > 0 && sampleCount != frameTimes.size()) {
            frameTimes.resize(sampleCount, 0);
            frameNbr = std::clamp(frameNbr, size_t(0), frameTimes.size());
            frameTimeIndex = std::clamp(frameTimeIndex, size_t(0), frameTimes.size() - 1);
        }
    }
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
    {}
    std::string         name;
    Instance::Handle    instance;
};
}

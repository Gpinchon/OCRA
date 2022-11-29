#include <Instance.hpp>
#include <PhysicalDevice.hpp>
#include <Device.hpp>

#include <Common.hpp>

#include <iostream>

using namespace OCRA;

static inline void ListQueues(const PhysicalDevice::Handle& a_PhysicalDevice, const Device::Handle& a_Device)
{
    PrintQueueInfos(a_PhysicalDevice);
    std::cout << "===== Device's Queues =====\n";
    for (const auto& queueInfo : GetQueueInfos(a_PhysicalDevice))
    {
        for (auto queueIndex = 0u; queueIndex < queueInfo.queueCount; ++queueIndex) {
            const auto queue = Device::GetQueue(a_Device, queueInfo.queueFamilyIndex, queueIndex);
            std::cout << "  == Queue ==\n";
            std::cout << "  Family    : " << queueInfo.queueFamilyIndex << "\n";
            std::cout << "  Index     : " << queueIndex << "\n";
            std::cout << "  Handle    : " << queue << "\n";
            std::cout << "  ===========\n";
        }
    }
    std::cout << "===========================\n";
    std::cout << "\n";
}

int main()
{
    const auto instance = CreateInstance("Test_Queue");
    const auto physicalDevice = Instance::EnumeratePhysicalDevices(instance).front();
    ListQueues(physicalDevice, CreateDevice(physicalDevice));
    return 0;
}
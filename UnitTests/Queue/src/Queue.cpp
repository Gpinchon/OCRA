#include <OCRA/OCRA.hpp>

#include <Common.hpp>

#include <iostream>

#include <gtest/gtest.h>

using namespace OCRA;

static inline void ListQueues(const PhysicalDevice::Handle& a_PhysicalDevice, const Device::Handle& a_Device)
{
    PrintQueueInfos(a_PhysicalDevice);
    for (const auto& queueInfo : GetQueueInfos(a_PhysicalDevice)) {
        for (auto queueIndex = 0u; queueIndex < queueInfo.queueCount; ++queueIndex) {
            const auto queue = Device::GetQueue(a_Device, queueInfo.queueFamilyIndex, queueIndex);
            ASSERT_NE(queue, nullptr);
        }
    }
}

TEST(QueueTests, ListQueues)
{
    const auto instance = CreateInstance("Test_Queue");
    ASSERT_NE(instance, nullptr);
    const auto physicalDevices = Instance::EnumeratePhysicalDevices(instance);
    ASSERT_FALSE(physicalDevices.empty());
    for (auto& physicalDevice : physicalDevices) {
        const auto physicalDevice = physicalDevices.front();
        ASSERT_NE(physicalDevice, nullptr);
        ListQueues(physicalDevice, CreateDevice(physicalDevice));
    }
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

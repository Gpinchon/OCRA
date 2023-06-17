#include <OCRA/OCRA.hpp>

#include <iostream>

#include <gtest/gtest.h>

using namespace OCRA;

int PhysicalDeviceTest()
{
    try {
        CreateInstanceInfo instanceInfo;
        instanceInfo.name               = "Test_PhysicalDevice";
        instanceInfo.applicationVersion = 1;
        const auto instance                             = CreateInstance(instanceInfo);
        SUCCEED() << "===== Physical Devices ====\n";
        for (const auto& physicalDevice : Instance::EnumeratePhysicalDevices(instance)) {
            const auto& properties = PhysicalDevice::GetProperties(physicalDevice);
            SUCCEED() << "  =============\n";
            SUCCEED() << "  Name        : " << properties.deviceName << "\n";
            SUCCEED() << "  Vendor      : " << properties.vendorName << "\n";
            SUCCEED() << "  API version : " << properties.apiVersion << "\n";
            SUCCEED() << "  =============\n";
        }
        SUCCEED() << "===========================\n";
    } catch (std::exception& e) {
        ADD_FAILURE() << e.what();
        return -1;
    }
    return 0;
}

int MultiInstanceTest()
{
    // test multi instances
    try {
        std::vector<Instance::Handle> instances;
        for (auto i = 0u; i < 2; ++i) {
            CreateInstanceInfo instanceInfo;
            instanceInfo.name               = "Test_PhysicalDevice_Instance_" + std::to_string(i);
            instanceInfo.applicationVersion = 1;
            instances.push_back(CreateInstance(instanceInfo));
            SUCCEED() << "Instance creation";
        }
        // if we did not crash yet, assume this test is successful
    } catch (std::exception& e) {
        ADD_FAILURE() << e.what();
        return -1;
    }
    SUCCEED() << "Multi instance destruction";
    return 0;
}

TEST(PhysicalDeviceTests, MultiInstance)
{
    ASSERT_EQ(MultiInstanceTest(), 0);
}

TEST(PhysicalDeviceTests, PhysicalDeviceCreation)
{
    ASSERT_EQ(PhysicalDeviceTest(), 0);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
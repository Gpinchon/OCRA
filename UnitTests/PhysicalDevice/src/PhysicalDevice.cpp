#include <OCRA/OCRA.hpp>

#include <iostream>

using namespace OCRA;

int main()
{
    //test multi instances
    {
        std::vector<Instance::Handle> instances;
        for (auto i = 0u; i < 2; ++i) {
            CreateInstanceInfo instanceInfo;
            instanceInfo.applicationInfo.name = "Test_PhysicalDevice_Instance_" + std::to_string(i);
            instanceInfo.applicationInfo.applicationVersion = 1;
            instances.push_back(CreateInstance(instanceInfo));
        }
        //if we did not crash yet, assume this test is successful
    }
    CreateInstanceInfo instanceInfo;
    instanceInfo.applicationInfo.name = "Test_PhysicalDevice";
    instanceInfo.applicationInfo.applicationVersion = 1;
    const auto instance = CreateInstance(instanceInfo);
    std::cout << "===== Physical Devices ====\n";
    for (const auto& physicalDevice : Instance::EnumeratePhysicalDevices(instance))
    {
        const auto& properties = PhysicalDevice::GetProperties(physicalDevice);
        std::cout << "  =============\n";
        std::cout << "  Name        : " << properties.deviceName << "\n";
        std::cout << "  Vendor      : " << properties.vendorName << "\n";
        std::cout << "  API version : " << properties.apiVersion << "\n";
        std::cout << "  =============\n";
    }
    std::cout << "===========================\n";
    return 0;
}
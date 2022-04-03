#include <Instance.hpp>
#include <PhysicalDevice.hpp>

#include <iostream>

using namespace OCRA;

//Create OCRA Instance
static inline auto CreateInstance()
{
	Instance::Handle instance;
	Instance::Info instanceInfo;
	instanceInfo.applicationInfo.name = "Test_PhysicalDevice";
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

int PhysicalDevice()
{
	const auto instance = CreateInstance();
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
	std::cout << std::endl;
	return 0;
}
#pragma once
/**
 * @brief Unit test's common tools
 */

#include <Handle.hpp>

#include <Common/Timer.hpp>

#include <iostream>
#include <string>

struct VerboseTimer : Timer
{
	VerboseTimer(const std::string& a_Name) : name(a_Name) {}
	~VerboseTimer() {
		std::cout << "Timer \"" << name << "\" waited : " << Elapsed().count() << " nanoseconds\n";
	}
	const std::string name;
};

OCRA_DECLARE_HANDLE(OCRA::Instance);
OCRA_DECLARE_HANDLE(OCRA::PhysicalDevice);
OCRA_DECLARE_HANDLE(OCRA::Device);

//Create an instance with app name a_Name
OCRA::Instance::Handle CreateInstance(const std::string& a_Name);
//Create a device with all available queues
OCRA::Device::Handle CreateDevice(const OCRA::PhysicalDevice::Handle& a_PhysicalDevice);


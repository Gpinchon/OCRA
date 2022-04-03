#include <Instance.hpp>
#include <PhysicalDevice.hpp>
#include <Device.hpp>
#include <Queue/Queue.hpp>
#include <Queue/Fence.hpp>
#include <Command/Pool.hpp>
#include <Command/Buffer.hpp>
#include <Memory.hpp>
#include <Buffer.hpp>

#include <Common.hpp>

#include <iostream>

using namespace OCRA;

//Create OCRA Instance
auto CreateInstance()
{
	Instance::Handle instance;
	Instance::Info instanceInfo;
	instanceInfo.applicationInfo.name = "Test_CommandBuffer";
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

//Get queues from Physical Device
auto GetQueueInfos(const PhysicalDevice::Handle& a_PhysicalDevice)
{
	std::vector<Queue::Info> queueInfos;
	{
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
	}
	return queueInfos;
}

//Find suitable device
auto GetPhysicalDevice(const Instance::Handle& a_Instance)
{
	const auto& physicalDevices = Instance::EnumeratePhysicalDevices(a_Instance);
	return physicalDevices.front();
}

//Create a device with necessary queues
auto CreateDevice(const PhysicalDevice::Handle& a_PhysicalDevice)
{
	Device::Info deviceInfo;
	deviceInfo.queueInfos = GetQueueInfos(a_PhysicalDevice);
	auto device = Device::Create(a_PhysicalDevice, deviceInfo);
	return device;
}

auto FindQueueFamily(const PhysicalDevice::Handle& a_PhysicalDevice, const PhysicalDevice::QueueFlags& a_QueueProperties)
{
	auto& queueProperties = PhysicalDevice::GetQueueFamilyProperties(a_PhysicalDevice);
	for (auto familyIndex = 0u; familyIndex < queueProperties.size(); ++familyIndex) {
		if (queueProperties.at(familyIndex).queueFlags == a_QueueProperties)
			return familyIndex;
	}
	return std::numeric_limits<uint32_t>::infinity();
}

auto FindProperMemoryType(const PhysicalDevice::Handle& a_PhysicalDevice, const PhysicalDevice::MemoryPropertyFlags& a_MemoryProperties)
{
	auto& memoryProperties = PhysicalDevice::GetMemoryProperties(a_PhysicalDevice);
	for (auto memoryTypeIndex = 0u; memoryTypeIndex < memoryProperties.memoryTypes.size(); ++memoryTypeIndex) {
		if (memoryProperties.memoryTypes.at(memoryTypeIndex).propertyFlags == a_MemoryProperties)
			return memoryTypeIndex;
	}
	return std::numeric_limits<uint32_t>::infinity();
}

#include <future>

void SubmitCommandBuffer(const Device::Handle& a_Device, const Queue::Handle& a_Queue, const Command::Buffer::Handle& a_CommandBuffer)
{
	auto fence = Queue::Fence::Create(a_Device);
	std::cout << "========== Command Buffer submit ==========\n";
	Queue::SubmitInfo submitInfo;
	submitInfo.commandBuffers.push_back(a_CommandBuffer);
	//test multithreaded submit
	std::async([a_Queue, submitInfo, fence] {
		Queue::Submit(a_Queue, { submitInfo }, fence);
	});
	//Queue::Submit(a_Queue, { submitInfo }, fence);
		
	//make sure GPU is done
	{
		VerboseTimer bufferCopiesTimer("Buffer Copies");
		Queue::Fence::WaitFor(a_Device, fence, std::chrono::nanoseconds(15000000));
	}
	//test for function time itself
	{
		auto timer = Timer();
		int waitNbr = 100000;
		for (auto i = 0; i < waitNbr; ++i)
			Queue::Fence::WaitFor(a_Device, fence, std::chrono::nanoseconds(15000000));
		std::cout << "Already signaled Fence mean wait time : " << timer.Elapsed().count() / double(waitNbr) << " nanoseconds\n";
	}
	std::cout << "===========================================\n";
	std::cout << "\n";
}

int CommandBuffer()
{
	auto instance = CreateInstance();
	auto physicalDevice = GetPhysicalDevice(instance);
	auto device = CreateDevice(physicalDevice);
	auto queueFamily = FindQueueFamily(physicalDevice, PhysicalDevice::QueueFlagsBits::Transfer);
	auto queue = Device::GetQueue(device, queueFamily, 0); //Get first available queue

	const std::string sentence0 = "Hello World !";
	const std::string sentence1 = "All your base are belong to us";

	std::cout << "========== Sentences to swap ==========\n";
	std::cout << "  Sentence 0 : " << sentence0 << "\n";
	std::cout << "  Sentence 1 : " << sentence1 << "\n";
	std::cout << "=======================================\n";
	std::cout << "\n";

	//allocate GPU memory
	constexpr auto chunkSize = sizeof(char) * 256;
	Memory::Info memoryInfo;
	memoryInfo.memoryTypeIndex = FindProperMemoryType(physicalDevice, PhysicalDevice::MemoryPropertyFlagBits::HostVisible | PhysicalDevice::MemoryPropertyFlagBits::HostCached);
	memoryInfo.size = chunkSize * 3;
	auto memory = Memory::Allocate(device, memoryInfo);
	//create test buffers
	Buffer::Info bufferInfo;
	bufferInfo.size = chunkSize;
	bufferInfo.usage = Buffer::UsageFlagBits::TransferDst | Buffer::UsageFlagBits::TransferSrc;
	auto buffer0 = Buffer::Create(device, bufferInfo);
	auto buffer1 = Buffer::Create(device, bufferInfo);
	auto buffer2 = Buffer::Create(device, bufferInfo);
	Buffer::BindMemory(device, buffer0, memory, chunkSize * 0);
	Buffer::BindMemory(device, buffer1, memory, chunkSize * 1);
	Buffer::BindMemory(device, buffer2, memory, chunkSize * 2);
	//write some value to the buffer0
	{
		auto bufferPtr = Memory::Map(device, memory, chunkSize * 0, chunkSize);
		memcpy(bufferPtr, sentence0.c_str(), sentence0.size());
		Memory::Unmap(device, memory);
	}
	//write some value to the buffer1
	{
		auto bufferPtr = Memory::Map(device, memory, chunkSize * 1, chunkSize);
		memcpy(bufferPtr, sentence1.c_str(), sentence1.size());
		Memory::Unmap(device, memory);
	}
	Command::Pool::Info commandPoolInfo;
	commandPoolInfo.queueFamilyIndex = queueFamily;
	auto commandPool = Command::Pool::Create(device, commandPoolInfo);
	Command::Buffer::AllocateInfo commandBufferAllocateInfo;
	commandBufferAllocateInfo.commandPool = commandPool;
	commandBufferAllocateInfo.count = 1;
	commandBufferAllocateInfo.level = Command::Buffer::Level::Primary;
	auto commandBuffer = Command::Buffer::Allocate(device, commandBufferAllocateInfo).front();
	Command::CommandBufferBeginInfo commandBufferBeginInfo;
	commandBufferBeginInfo.flags = Command::CommandBufferUsageFlagBits::OneTimeSubmit;
	Command::BeginCommandBuffer(commandBuffer, commandBufferBeginInfo);
	{
		Command::BufferCopyRegion copyRegions;
		copyRegions.size = chunkSize;
		Command::CopyBuffer(commandBuffer, buffer0, buffer2, { copyRegions });
		Command::CopyBuffer(commandBuffer, buffer1, buffer0, { copyRegions });
		Command::CopyBuffer(commandBuffer, buffer2, buffer1, { copyRegions });
	}
	Command::EndCommandBuffer(commandBuffer);

	SubmitCommandBuffer(device, queue, commandBuffer);
	
	std::cout << "===== Check if sentences were swapped =====\n";
	int success = 0;
	{
		std::string buffer0String = (char*)Memory::Map(device, memory, chunkSize * 0, chunkSize);
		success += buffer0String == sentence1 ? 0 : 1;
		std::cout << "  Buffer 0 value : " << buffer0String << "\n";
		Memory::Unmap(device, memory);
	}
	{
		std::string buffer1String = (char*)Memory::Map(device, memory, chunkSize * 1, chunkSize);
		success += buffer1String == sentence0 ? 0 : 1;
		std::cout << "  Buffer 1 value : " << buffer1String << "\n";
		Memory::Unmap(device, memory);
	}
	std::cout << "    " << (success == 0 ? "***** Great success ! *****" : "XXXXX Failure will not be tolerated. XXXXX") << "\n";
	std::cout << "===========================================\n";
	std::cout << std::endl;
	return success;
}
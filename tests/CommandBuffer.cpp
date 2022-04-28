#include <Instance.hpp>
#include <PhysicalDevice.hpp>
#include <Device.hpp>
#include <Queue/Queue.hpp>
#include <Queue/Fence.hpp>
#include <Queue/Semaphore.hpp>
#include <Command/Pool.hpp>
#include <Command/Buffer.hpp>
#include <Memory.hpp>
#include <Buffer.hpp>

#include <Common.hpp>

#include <future>
#include <iostream>

using namespace OCRA;

//Find suitable device
static inline auto GetPhysicalDevice(const Instance::Handle& a_Instance)
{
	const auto& physicalDevices = Instance::EnumeratePhysicalDevices(a_Instance);
	return physicalDevices.front();
}

static inline auto FindProperMemoryType(const PhysicalDevice::Handle& a_PhysicalDevice, const PhysicalDevice::MemoryPropertyFlags& a_MemoryProperties)
{
	auto& memoryProperties = PhysicalDevice::GetMemoryProperties(a_PhysicalDevice);
	for (auto memoryTypeIndex = 0u; memoryTypeIndex < memoryProperties.memoryTypes.size(); ++memoryTypeIndex) {
		if (memoryProperties.memoryTypes.at(memoryTypeIndex).propertyFlags == a_MemoryProperties)
			return memoryTypeIndex;
	}
	return std::numeric_limits<uint32_t>::infinity();
}

#define FENCE_VERSION

static inline void SubmitCommandBuffer(const Device::Handle& a_Device, const Queue::Handle& a_Queue, const Command::Buffer::Handle& a_CommandBuffer)
{
#ifndef FENCE_VERSION
	Queue::Semaphore::Info semaphoreInfo;
	semaphoreInfo.type = Queue::Semaphore::Type::Timeline;
	semaphoreInfo.initialValue = 0;
	Queue::Semaphore::Handle semaphore = Queue::Semaphore::Create(a_Device, semaphoreInfo);
	Queue::TimelineSemaphoreSubmitInfo timelineValues;
	timelineValues.waitSemaphoreValues.push_back(1);
	timelineValues.signalSemaphoreValues.push_back(2);
#else
	auto fence = Queue::Fence::Create(a_Device);
#endif
	Queue::SubmitInfo submitInfo;
	for (auto i = 0u; i < 1; ++i)
		submitInfo.commandBuffers.push_back(a_CommandBuffer);
#ifndef FENCE_VERSION
	//submitInfo.waitSemaphores.push_back(semaphore);
	submitInfo.signalSemaphores.push_back(semaphore);
	submitInfo.timelineSemaphoreValues = timelineValues;
#endif
	std::cout << "========== Command Buffer submit ==========\n";
	//test multithreaded submit
#ifndef FENCE_VERSION
	std::async([a_Queue, submitInfo] {
		Queue::Submit(a_Queue, { submitInfo });
	});
	//Queue::Semaphore::Signal(a_Device, semaphore, 1);
#else
	auto lambada = [a_Queue] {};
	std::function<void()> lambadaFun(lambada);
	auto future = std::async([a_Queue, submitInfo, fence] {
		VerboseTimer("Queue Submission");
		Queue::Submit(a_Queue, { submitInfo }, fence);
	});
#endif
	//make sure GPU is done
	{
		VerboseTimer bufferCopiesTimer("Buffer Copies");
#ifndef FENCE_VERSION
		Queue::Semaphore::Wait(a_Device, { semaphore }, { 2 }, std::chrono::nanoseconds(15000000));
#else
		Queue::Fence::WaitFor(a_Device, fence, std::chrono::nanoseconds(15000000));
#endif
	}
	//test for function time itself
#ifdef FENCE_VERSION
	{
		auto timer = Timer();
		int waitNbr = 100000;
		for (auto i = 0; i < waitNbr; ++i)
			Queue::Fence::WaitFor(a_Device, fence, std::chrono::nanoseconds(15000000));
		std::cout << "Already signaled Fence mean wait time : " << timer.Elapsed().count() / double(waitNbr) << " nanoseconds\n";
	}
#endif
	std::cout << "===========================================\n";
	std::cout << "\n";
}

int CommandBuffer()
{
	auto instance = CreateInstance("Test_CommandBuffer");
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
		Memory::MappedRange mappedRange;
		mappedRange.memory = memory;
		mappedRange.offset = chunkSize * 0;
		mappedRange.length = chunkSize;
		auto bufferPtr = Memory::Map(device, mappedRange);
		memcpy(bufferPtr, sentence0.c_str(), sentence0.size());
		Memory::Unmap(device, memory);
	}
	//write some value to the buffer1
	{
		Memory::MappedRange mappedRange;
		mappedRange.memory = memory;
		mappedRange.offset = chunkSize * 1;
		mappedRange.length = chunkSize;
		auto bufferPtr = Memory::Map(device, mappedRange);
		memcpy(bufferPtr, sentence1.c_str(), sentence1.size());
		Memory::Unmap(device, memory);
	}
	Command::Pool::Info commandPoolInfo;
	commandPoolInfo.queueFamilyIndex = queueFamily;
	auto commandPool = Command::Pool::Create(device, commandPoolInfo);
	Command::Pool::AllocateInfo commandBufferAllocateInfo;
	commandBufferAllocateInfo.pool = commandPool;
	commandBufferAllocateInfo.count = 1;
	commandBufferAllocateInfo.level = Command::Pool::AllocateInfo::Level::Primary;
	auto commandBuffer = Command::Pool::AllocateBuffer(device, commandBufferAllocateInfo).front();
	Command::CommandBufferBeginInfo commandBufferBeginInfo;
	commandBufferBeginInfo.flags = Command::CommandBufferUsageFlagBits::None;
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
		Memory::MappedRange mappedRange;
		mappedRange.memory = memory;
		mappedRange.offset = chunkSize * 0;
		mappedRange.length = chunkSize;
		std::string buffer0String = (char*)Memory::Map(device, mappedRange);
		success += buffer0String == sentence1 ? 0 : 1;
		std::cout << "  Buffer 0 value : " << buffer0String << "\n";
		Memory::Unmap(device, memory);
	}
	{
		Memory::MappedRange mappedRange;
		mappedRange.memory = memory;
		mappedRange.offset = chunkSize * 1;
		mappedRange.length = chunkSize;
		std::string buffer1String = (char*)Memory::Map(device, mappedRange);
		success += buffer1String == sentence0 ? 0 : 1;
		std::cout << "  Buffer 1 value : " << buffer1String << "\n";
		Memory::Unmap(device, memory);
	}
	std::cout << "    " << (success == 0 ? "***** Great success ! *****" : "XXXXX Failure will not be tolerated. XXXXX") << "\n";
	std::cout << "===========================================\n";
	std::cout << std::endl;
	return success;
}
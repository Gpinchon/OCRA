#include <OCRA/Instance.hpp>
#include <OCRA/PhysicalDevice.hpp>
#include <OCRA/Device.hpp>
#include <OCRA/Queue/Queue.hpp>
#include <OCRA/Queue/Fence.hpp>
#include <OCRA/Queue/Semaphore.hpp>
#include <OCRA/Command/Pool.hpp>
#include <OCRA/Command/Buffer.hpp>
#include <OCRA/Memory.hpp>
#include <OCRA/Buffer.hpp>

#include <Common.hpp>

#include <future>
#include <iostream>

#define FENCE_VERSION
#define CHUNK_SIZE 256
#define SENTENCE0  std::string("Hello World !")
#define SENTENCE1  std::string("All your base are belong to us")
#define SWAP_NBR   101

using namespace OCRA;

#ifdef FENCE_VERSION
static inline void SubmitCommandBuffer(const Device::Handle& a_Device, const Queue::Handle& a_Queue, const Command::Buffer::Handle& a_CommandBuffer)
{
    auto fence = Queue::Fence::Create(a_Device);
    Queue::SubmitInfo submitInfo;
    for (auto i = 0u; i < SWAP_NBR; ++i)
        submitInfo.commandBuffers.push_back(a_CommandBuffer);
    std::cout << "========== Command Buffer submit ==========\n";
    //test multithreaded submit
    std::async([a_Queue, submitInfo, fence] {
        VerboseTimer("Queue Submission");
        Queue::Submit(a_Queue, { submitInfo }, fence);
    });
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
#else //FENCE_VERSION
static inline void SubmitCommandBuffer(const Device::Handle& a_Device, const Queue::Handle& a_Queue, const Command::Buffer::Handle& a_CommandBuffer)
{
    Queue::Semaphore::Info semaphoreInfo;
    semaphoreInfo.type = Queue::Semaphore::Type::Timeline;
    semaphoreInfo.initialValue = 0;
    Queue::Semaphore::Handle semaphore = Queue::Semaphore::Create(a_Device, semaphoreInfo);
    Queue::TimelineSemaphoreSubmitInfo timelineValues;
    timelineValues.waitSemaphoreValues.push_back(1);
    timelineValues.signalSemaphoreValues.push_back(2);
    Queue::SubmitInfo submitInfo;
    for (auto i = 0u; i < SWAP_NBR; ++i)
        submitInfo.commandBuffers.push_back(a_CommandBuffer);
    submitInfo.waitSemaphores.push_back(semaphore);
    submitInfo.signalSemaphores.push_back(semaphore);
    submitInfo.timelineSemaphoreValues = timelineValues;
    std::cout << "========== Command Buffer submit ==========\n";
    //test multithreaded submit
    std::async([a_Queue, submitInfo] {
        VerboseTimer("Queue Submission");
        Queue::Submit(a_Queue, { submitInfo });
    });
    Queue::Semaphore::Signal(a_Device, semaphore, 1);
    //make sure GPU is done
    {
        VerboseTimer bufferCopiesTimer("Buffer Copies");
        Queue::Semaphore::Wait(a_Device, { semaphore }, { 2 }, std::chrono::nanoseconds(15000000));
    }
    std::cout << "===========================================\n";
    std::cout << "\n";
}
#endif

static inline void RecordSwapCommandBuffer(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Buffer::Handle& a_Buffer0,
    const Buffer::Handle& a_Buffer1,
    const Buffer::Handle& a_BufferT)
{
    Command::Buffer::BeginInfo bufferbeginInfo;
    bufferbeginInfo.flags = Command::Buffer::UsageFlagBits::None;
    Command::Buffer::Begin(a_CommandBuffer, bufferbeginInfo);
    {
        Command::BufferCopyRegion copyRegions;
        copyRegions.size = CHUNK_SIZE;
        Command::CopyBuffer(a_CommandBuffer, a_Buffer0, a_BufferT, { copyRegions });
        Command::CopyBuffer(a_CommandBuffer, a_Buffer1, a_Buffer0, { copyRegions });
        Command::CopyBuffer(a_CommandBuffer, a_BufferT, a_Buffer1, { copyRegions });
    }
    Command::Buffer::End(a_CommandBuffer);
}

int main()
{
    const auto instance = CreateInstance("Test_CommandBuffer");
    const auto physicalDevice = Instance::EnumeratePhysicalDevices(instance).front();
    const auto device = CreateDevice(physicalDevice);
    const auto queueFamily = FindQueueFamily(physicalDevice, PhysicalDevice::QueueFlagsBits::Transfer);
    const auto queue = Device::GetQueue(device, queueFamily, 0); //Get first available queue
    const auto memory = AllocateMemory(physicalDevice, device, CHUNK_SIZE * 3, PhysicalDevice::MemoryPropertyFlagBits::HostVisible | PhysicalDevice::MemoryPropertyFlagBits::HostCached);
    const auto commandPool = CreateCommandPool(device, queueFamily);
    const auto commandBuffer = CreateCommandBuffer(device, commandPool, Command::Pool::AllocateInfo::Level::Primary);

    //create test buffers
    Buffer::Info bufferInfo;
    bufferInfo.size = CHUNK_SIZE;
    bufferInfo.usage = Buffer::UsageFlagBits::TransferDst | Buffer::UsageFlagBits::TransferSrc;
    const auto buffer0 = Buffer::Create(device, bufferInfo);
    const auto buffer1 = Buffer::Create(device, bufferInfo);
    const auto bufferT = Buffer::Create(device, bufferInfo);
    Buffer::BindMemory(device, buffer0, memory, CHUNK_SIZE * 0);
    Buffer::BindMemory(device, buffer1, memory, CHUNK_SIZE * 1);
    Buffer::BindMemory(device, bufferT, memory, CHUNK_SIZE * 2);
    //write some value to the buffer0
    {
        Memory::MappedRange mappedRange;
        mappedRange.memory = memory;
        mappedRange.offset = CHUNK_SIZE * 0;
        mappedRange.length = CHUNK_SIZE;
        auto bufferPtr = Memory::Map(device, mappedRange);
        memcpy(bufferPtr, SENTENCE0.c_str(), SENTENCE0.size());
        Memory::Unmap(device, memory);
    }
    //write some value to the buffer1
    {
        Memory::MappedRange mappedRange;
        mappedRange.memory = memory;
        mappedRange.offset = CHUNK_SIZE * 1;
        mappedRange.length = CHUNK_SIZE;
        auto bufferPtr = Memory::Map(device, mappedRange);
        memcpy(bufferPtr, SENTENCE1.c_str(), SENTENCE1.size());
        Memory::Unmap(device, memory);
    }
    RecordSwapCommandBuffer(commandBuffer, buffer0, buffer1, bufferT);
    SubmitCommandBuffer(device, queue, commandBuffer);

    std::cout << "========== Sentences to swap ==========\n";
    std::cout << "  Sentence 0 : " << SENTENCE0 << "\n";
    std::cout << "  Sentence 1 : " << SENTENCE1 << "\n";
    std::cout << "=======================================\n";
    std::cout << "\n";
    std::cout << "===== Check if sentences were swapped =====\n";
    int success = 0;
    {
        Memory::MappedRange mappedRange;
        mappedRange.memory = memory;
        mappedRange.offset = CHUNK_SIZE * 0;
        mappedRange.length = CHUNK_SIZE;
        std::string buffer0String = (char*)Memory::Map(device, mappedRange);
        success += buffer0String == SENTENCE1 ? 0 : 1;
        std::cout << "  Buffer 0 value : " << buffer0String << "\n";
        Memory::Unmap(device, memory);
    }
    {
        Memory::MappedRange mappedRange;
        mappedRange.memory = memory;
        mappedRange.offset = CHUNK_SIZE * 1;
        mappedRange.length = CHUNK_SIZE;
        std::string buffer1String = (char*)Memory::Map(device, mappedRange);
        success += buffer1String == SENTENCE0 ? 0 : 1;
        std::cout << "  Buffer 1 value : " << buffer1String << "\n";
        Memory::Unmap(device, memory);
    }
    std::cout << "    " << (success == 0 ? "***** Great success ! *****" : "XXXXX Failure will not be tolerated. XXXXX") << "\n";
    std::cout << "===========================================\n";
    return success;
}
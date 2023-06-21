#include <OCRA/OCRA.hpp>

#include <Common.hpp>

#include <future>

#include <gtest/gtest.h>

#define FENCE_VERSION
#define CHUNK_SIZE 256
#define SENTENCE0  std::string("Hello World !")
#define SENTENCE1  std::string("All your base are belong to us")
#define SWAP_NBR   101

using namespace OCRA;

static inline void SubmitCommandBufferFence(
    const Device::Handle& a_Device,
    const Queue::Handle& a_Queue,
    const Command::Buffer::Handle& a_CommandBuffer)
{
    auto fence = CreateFence(a_Device);
    QueueSubmitInfo submitInfo;
    for (auto i = 0u; i < SWAP_NBR; ++i)
        submitInfo.commandBuffers.push_back(a_CommandBuffer);
    {
        VerboseTimer("Queue Submission");
        Queue::Submit(a_Queue, { submitInfo }, fence);
    }
    //make sure GPU is done
    {
        VerboseTimer bufferCopiesTimer("Buffer Copies");
        Fence::WaitFor(fence, IgnoreTimeout);
    }
    //test for function time itself
    {
        auto timer = Timer();
        int waitNbr = 100;
        for (auto i = 0; i < waitNbr; ++i) {
            Fence::WaitFor(fence, IgnoreTimeout);
        }
    }
}

static inline void SubmitCommandBufferSemaphore(
    const Device::Handle& a_Device,
    const Queue::Handle& a_Queue,
    const Command::Buffer::Handle& a_CommandBuffer)
{
    CreateSemaphoreInfo semaphoreInfo;
    semaphoreInfo.type = SemaphoreType::Timeline;
    semaphoreInfo.initialValue = 0;
    Semaphore::Handle semaphore = Device::CreateSemaphore(a_Device, semaphoreInfo);
    TimelineSemaphoreSubmitInfo timelineValues;
    timelineValues.waitSemaphoreValues.push_back(1);
    timelineValues.signalSemaphoreValues.push_back(2);
    QueueSubmitInfo submitInfo;
    for (auto i = 0u; i < SWAP_NBR; ++i)
        submitInfo.commandBuffers.push_back(a_CommandBuffer);
    QueueSubmitWaitInfo waitInfo;
    waitInfo.semaphore = semaphore;
    waitInfo.timelineValue = 1;
    waitInfo.dstStages = PipelineStageFlagBits::Transfer;
    QueueSubmitSignalInfo signalInfo;
    signalInfo.semaphore = semaphore;
    signalInfo.timelineValue = 2;
    submitInfo.waitSemaphores.push_back(waitInfo);
    submitInfo.signalSemaphores.push_back(signalInfo);
    //test multithreaded submit
    std::async([a_Queue, submitInfo] {
        VerboseTimer("Queue Submission");
        Queue::Submit(a_Queue, { submitInfo });
    });
    Semaphore::Signal(semaphore, 1);
    //make sure GPU is done
    {
        VerboseTimer bufferCopiesTimer("Buffer Copies");
        Semaphore::Wait({ semaphore }, { 2 }, std::chrono::nanoseconds(15000000));
    }
}

static inline void RecordSwapCommandBuffer(
    const Command::Buffer::Handle& a_CommandBuffer,
    const Buffer::Handle& a_Buffer0,
    const Buffer::Handle& a_Buffer1,
    const Buffer::Handle& a_BufferT)
{
    BufferMemoryBarrier bufferMemoryBarrier;
    bufferMemoryBarrier.dstAccessMask = AccessFlagBits::TransferWrite;
    bufferMemoryBarrier.srcAccessMask = AccessFlagBits::TransferRead;
    bufferMemoryBarrier.size = CHUNK_SIZE;
    CommandBufferBeginInfo bufferbeginInfo;
    bufferbeginInfo.flags = CommandBufferUsageFlagBits::SimultaneousUse;
    Command::Buffer::Begin(a_CommandBuffer, bufferbeginInfo);
    {
        BufferCopyRegion copyRegions;
        copyRegions.size = CHUNK_SIZE;
        Command::CopyBuffer(a_CommandBuffer, a_Buffer0, a_BufferT, { copyRegions });
        {
            bufferMemoryBarrier.buffer = a_Buffer0;
            Command::PipelineBarrier(a_CommandBuffer,
                PipelineStageFlagBits::Transfer, PipelineStageFlagBits::Transfer,
                DependencyFlagBits::None,
                {}, { bufferMemoryBarrier }, {});
        }
        
        Command::CopyBuffer(a_CommandBuffer, a_Buffer1, a_Buffer0, { copyRegions });
        {
            bufferMemoryBarrier.buffer = a_Buffer1;
            Command::PipelineBarrier(a_CommandBuffer,
                PipelineStageFlagBits::Transfer, PipelineStageFlagBits::Transfer,
                DependencyFlagBits::None,
                {}, { bufferMemoryBarrier }, {});
        }
        Command::CopyBuffer(a_CommandBuffer, a_BufferT, a_Buffer1, { copyRegions });
        {
            bufferMemoryBarrier.buffer = a_BufferT;
            Command::PipelineBarrier(a_CommandBuffer,
                PipelineStageFlagBits::Transfer, PipelineStageFlagBits::Transfer,
                DependencyFlagBits::None,
                {}, { bufferMemoryBarrier }, {});
        }
    }
    Command::Buffer::End(a_CommandBuffer);
}

int TestBufferSwap(bool useFence)
{
    const auto instance = CreateInstance("Test_CommandBuffer");
    const auto physicalDevice = Instance::EnumeratePhysicalDevices(instance).front();
    const auto device = CreateDevice(physicalDevice);
    const auto queueFamily = PhysicalDevice::FindQueueFamily(physicalDevice, QueueFlagBits::Transfer);
    const auto queue = Device::GetQueue(device, queueFamily, 0); //Get first available queue
    const auto memory = Device::AllocateMemory(device, CHUNK_SIZE * 3,
        MemoryPropertyFlagBits::HostVisible | MemoryPropertyFlagBits::HostCached);
    const auto commandPool = CreateCommandPool(device, queueFamily);
    const auto commandBuffer = CreateCommandBuffer(commandPool, CommandBufferLevel::Primary);

    //create test buffers
    CreateBufferInfo bufferInfo;
    bufferInfo.size = CHUNK_SIZE;
    bufferInfo.usage = BufferUsageFlagBits::Transfer;
    const auto buffer0 = CreateBuffer(device, bufferInfo);
    const auto buffer1 = CreateBuffer(device, bufferInfo);
    const auto bufferT = CreateBuffer(device, bufferInfo);
    Buffer::BindMemory(buffer0, memory, CHUNK_SIZE * 0);
    Buffer::BindMemory(buffer1, memory, CHUNK_SIZE * 1);
    Buffer::BindMemory(bufferT, memory, CHUNK_SIZE * 2);
    //write some value to the buffer0
    {
        MemoryMappedRange mappedRange;
        mappedRange.memory = memory;
        mappedRange.offset = CHUNK_SIZE * 0;
        mappedRange.length = CHUNK_SIZE;
        auto bufferPtr = Memory::Map(mappedRange);
        memcpy(bufferPtr, SENTENCE0.c_str(), SENTENCE0.size());
        Memory::Unmap(memory);
    }
    //write some value to the buffer1
    {
        MemoryMappedRange mappedRange;
        mappedRange.memory = memory;
        mappedRange.offset = CHUNK_SIZE * 1;
        mappedRange.length = CHUNK_SIZE;
        auto bufferPtr = Memory::Map(mappedRange);
        memcpy(bufferPtr, SENTENCE1.c_str(), SENTENCE1.size());
        Memory::Unmap(memory);
    }
    RecordSwapCommandBuffer(commandBuffer, buffer0, buffer1, bufferT);
    if (useFence)
        SubmitCommandBufferFence(device, queue, commandBuffer);
    else SubmitCommandBufferSemaphore(device, queue, commandBuffer);

    int success = 0;
    {
        MemoryMappedRange mappedRange;
        mappedRange.memory = memory;
        mappedRange.offset = CHUNK_SIZE * 0;
        mappedRange.length = CHUNK_SIZE;
        std::string buffer0String = (char*)Memory::Map(mappedRange);
        EXPECT_EQ(buffer0String, SENTENCE1);
        success += buffer0String == SENTENCE1 ? 0 : 1;
        Memory::Unmap(memory);
    }
    {
        MemoryMappedRange mappedRange;
        mappedRange.memory = memory;
        mappedRange.offset = CHUNK_SIZE * 1;
        mappedRange.length = CHUNK_SIZE;
        std::string buffer1String = (char*)Memory::Map(mappedRange);
        EXPECT_EQ(buffer1String, SENTENCE0);
        success += buffer1String == SENTENCE0 ? 0 : 1;
        Memory::Unmap(memory);
    }
    return success;
}

TEST(CommandBuffer, Creation)
{
    CreateInstanceInfo instanceInfo;
    instanceInfo.name = "Test_CommandBuffer";
    instanceInfo.applicationVersion = 1;
    const auto instance = CreateInstance(instanceInfo);
    ASSERT_NE(instance, nullptr);
    const auto physicalDevice = Instance::EnumeratePhysicalDevices(instance).front();
    ASSERT_NE(physicalDevice, nullptr);
    const auto device = CreateDevice(physicalDevice);
    ASSERT_NE(device, nullptr);
    const auto queueFamily = PhysicalDevice::FindQueueFamily(physicalDevice, QueueFlagBits::None);
    const auto queue = Device::GetQueue(device, queueFamily, 0); //Get first available queue
    ASSERT_NE(queue, nullptr);
    const auto commandPool = CreateCommandPool(device, queueFamily);
    ASSERT_NE(commandPool, nullptr);
    const auto commandBuffer = CreateCommandBuffer(commandPool, CommandBufferLevel::Primary);
    ASSERT_NE(commandBuffer, nullptr);
}

TEST(CommandBuffer, SwapWithFence)
{
    ASSERT_EQ(TestBufferSwap(true), 0);
}

TEST(CommandBuffer, SwapWithSemaphore)
{
    ASSERT_EQ(TestBufferSwap(false), 0);
}

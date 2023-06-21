#include <OCRA/OCRA.hpp>

#include <Common.hpp>

#include <iomanip>
#include <algorithm>
#include <random>

#include <gtest/gtest.h>

using namespace OCRA;

constexpr auto TEST_COUNT = 100;
constexpr auto DESCRIPTOR_COUNT = 1024;

void DescriptorTest(const PhysicalDevice::Handle& a_PhysicalDevice, const Device::Handle& a_Device, const std::vector<size_t>& a_IterationIndice)
{
    double CreatingTimer = 0;
    double DeletingTimer = 0;
    double ReCreatingTimer = 0;
    double UpdatingTimer = 0;

    CreateDescriptorPoolInfo poolInfo;
    poolInfo.maxSets = DESCRIPTOR_COUNT;
    poolInfo.sizes.push_back({ DescriptorType::UniformBuffer, DESCRIPTOR_COUNT });
    auto descriptorPool = CreateDescriptorPool(a_Device, poolInfo);
    DescriptorSetBinding bindingLayout;
    bindingLayout.binding = 0;
    bindingLayout.count = 1;
    bindingLayout.type = DescriptorType::UniformBuffer;
    bindingLayout.stageFlags = ShaderStageFlagBits::AllGraphics;
    AllocateDescriptorSetInfo allocateInfo;
    allocateInfo.bindings = { bindingLayout };

    for (int j = 0; j < TEST_COUNT; ++j)
    {
        std::vector<Descriptor::Set::Handle> descriptorSets(DESCRIPTOR_COUNT);
        {
            Timer timer;
            for (int i = 0; i < DESCRIPTOR_COUNT; ++i) {
                descriptorSets.at(i) = Descriptor::Pool::AllocateDescriptorSet(descriptorPool, allocateInfo);
            }
            CreatingTimer += timer.Elapsed().count() / double(TEST_COUNT);
        }
        {
            Timer timer;
            for (const auto& i : a_IterationIndice) {
                if (i % 3 == 0) descriptorSets.at(i).reset();
            }
            DeletingTimer += timer.Elapsed().count() / double(TEST_COUNT);
        }
        {
            Timer timer;
            for (const auto& i : a_IterationIndice) {
                if (i % 3 == 0) descriptorSets.at(i) = Descriptor::Pool::AllocateDescriptorSet(descriptorPool, allocateInfo);
            }
            ReCreatingTimer += timer.Elapsed().count() / double(TEST_COUNT);
        }
        {
            AllocateBufferInfo bufferInfo;
            bufferInfo.memoryFlags = MemoryPropertyFlagBits::DeviceLocal;
            bufferInfo.bufferFlags = CreateBufferFlagBits::None;
            bufferInfo.bufferUsage = BufferUsageFlagBits::UniformBuffer;
            bufferInfo.size = 1024;
            auto buffer = AllocateBuffer(a_Device, bufferInfo);
            std::vector<DescriptorSetWrite> writeOperations(DESCRIPTOR_COUNT);
            for (const auto& i : a_IterationIndice) {
                DescriptorSetBufferInfo bufferInfo;
                bufferInfo.buffer = buffer;
                bufferInfo.range  = 1024;
                writeOperations.at(i).dstCount = 1;
                writeOperations.at(i).dstSet = descriptorSets.at(i);
                writeOperations.at(i).type = DescriptorType::UniformBuffer;
                writeOperations.at(i).bufferInfo = bufferInfo;
            }
            Timer timer;
            Descriptor::Set::Update(writeOperations, {});
            UpdatingTimer += timer.Elapsed().count() / double(TEST_COUNT);
        }
    }
    std::cout << "CreatingTimer   took " << std::setw(10) << unsigned(CreatingTimer) << " nanoseconds\n";
    std::cout << "DeletingTimer   took " << std::setw(10) << unsigned(DeletingTimer) << " nanoseconds\n";
    std::cout << "ReCreatingTimer took " << std::setw(10) << unsigned(ReCreatingTimer) << " nanoseconds\n";
    std::cout << "UpdatingTimer   took " << std::setw(10) << unsigned(UpdatingTimer) << " nanoseconds\n";
}

int DescriptorSetTest(bool a_Sequential)
{
    const auto instance = CreateInstance("Test_DescriptorSets");
    const auto physicalDevice = Instance::EnumeratePhysicalDevices(instance).front();
    const auto device = CreateDevice(physicalDevice);
    std::vector<size_t> iterationIndice(DESCRIPTOR_COUNT);
    for (size_t i = 0; i < DESCRIPTOR_COUNT; ++i) {
        iterationIndice.at(i) = i;
    }
    if (a_Sequential) {
        try {
            DescriptorTest(physicalDevice, device, iterationIndice);
        } catch (std::exception&) {
            return -1;
        }
    }
    else {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(iterationIndice.begin(), iterationIndice.end(), g);
        try {
            DescriptorTest(physicalDevice, device, iterationIndice);
        } catch (std::exception&) {
            return -1;
        }
        
    }
    return 0;
}

TEST(DescriptorSet, SequentialTest)
{
    ASSERT_EQ(DescriptorSetTest(true), 0);
}

TEST(DescriptorSet, RandomTest)
{
    ASSERT_EQ(DescriptorSetTest(false), 0);
}

#include <OCRA/OCRA.hpp>

#include <Common.hpp>

#include <iomanip>
#include <algorithm>
#include <random>

using namespace OCRA;

constexpr auto TEST_COUNT = 100;
constexpr auto DESCRIPTOR_COUNT = 1024;

void DescriptorTest(const PhysicalDevice::Handle& a_PhysicalDevice, const Device::Handle& a_Device, const std::vector<size_t>& a_IterationIndice)
{
    double CreatingTimer = 0;
    double DeletingTimer = 0;
    double ReCreatingTimer = 0;
    double UpdatingTimer = 0;

    Descriptor::Pool::Info poolInfo;
    poolInfo.maxSets = DESCRIPTOR_COUNT;
    poolInfo.sizes.push_back({ Descriptor::Type::UniformBuffer, DESCRIPTOR_COUNT });
    auto descriptorPool = Descriptor::Pool::Create(a_Device, poolInfo);
    Descriptor::SetLayout::Info layoutInfo;
    layoutInfo.bindings.push_back({ 0, Descriptor::Type::UniformBuffer, 1 });
    auto descriptorLayout = Descriptor::SetLayout::Create(a_Device, layoutInfo);
    Descriptor::Pool::AllocateInfo allocateInfo;
    allocateInfo.layout = descriptorLayout;
    allocateInfo.pool = descriptorPool;

    for (int j = 0; j < TEST_COUNT; ++j)
    {
        std::vector<Descriptor::Set::Handle> descriptorSets(DESCRIPTOR_COUNT);
        {
            Timer timer;
            for (int i = 0; i < DESCRIPTOR_COUNT; ++i) {
                descriptorSets.at(i) = Descriptor::Pool::AllocateSet(a_Device, allocateInfo);
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
                if (i % 3 == 0) descriptorSets.at(i) = Descriptor::Pool::AllocateSet(a_Device, allocateInfo);
            }
            ReCreatingTimer += timer.Elapsed().count() / double(TEST_COUNT);
        }
        {
            Buffer::Info bufferInfo;
            bufferInfo.usage = Buffer::UsageFlagBits::UniformBuffer;
            bufferInfo.size = 1024;
            auto memory = AllocateMemory(a_PhysicalDevice, a_Device, 1024, PhysicalDevice::MemoryPropertyFlagBits::DeviceLocal);
            auto buffer = Buffer::Create(a_Device, bufferInfo);
            Buffer::BindMemory(a_Device, buffer, memory, 0);
            std::vector<Descriptor::Set::WriteOperation> writeOperations(DESCRIPTOR_COUNT);
            for (const auto& i : a_IterationIndice) {
                Descriptor::Set::BufferInfo bufferInfo;
                bufferInfo.buffer = buffer;
                writeOperations.at(i).dstSet = descriptorSets.at(i);
                writeOperations.at(i).type = Descriptor::Type::UniformBuffer;
                writeOperations.at(i).bufferInfo = bufferInfo;
            }
            Timer timer;
            Descriptor::Set::Update(a_Device, writeOperations, {});
            UpdatingTimer += timer.Elapsed().count() / double(TEST_COUNT);
        }
    }
    std::cout << "CreatingTimer   took " << std::setw(10) << unsigned(CreatingTimer) << " nanoseconds\n";
    std::cout << "DeletingTimer   took " << std::setw(10) << unsigned(DeletingTimer) << " nanoseconds\n";
    std::cout << "ReCreatingTimer took " << std::setw(10) << unsigned(ReCreatingTimer) << " nanoseconds\n";
    std::cout << "UpdatingTimer   took " << std::setw(10) << unsigned(UpdatingTimer) << " nanoseconds\n";
}

int main(int argc, char const *argv[])
{
    const auto instance = CreateInstance("Test_CommandBuffer");
    const auto physicalDevice = Instance::EnumeratePhysicalDevices(instance).front();
    const auto device = CreateDevice(physicalDevice);
    std::vector<size_t> iterationIndice(DESCRIPTOR_COUNT);
    for (size_t i = 0; i < DESCRIPTOR_COUNT; ++i) {
        iterationIndice.at(i) = i;
    }

    std::cout << "--------------------------------------------------------------------------------\n";
    std::cout << "Sequential indice test \n";
    std::cout << "--------------------------------------------------------------------------------\n";
    DescriptorTest(physicalDevice, device, iterationIndice);
    std::cout << "--------------------------------------------------------------------------------\n";
    std::cout << "\n";

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(iterationIndice.begin(), iterationIndice.end(), g);
    std::cout << "--------------------------------------------------------------------------------\n";
    std::cout << "Random indice test \n";
    std::cout << "--------------------------------------------------------------------------------\n";
    DescriptorTest(physicalDevice, device, iterationIndice);
    std::cout << "--------------------------------------------------------------------------------\n";

    return 0;
}
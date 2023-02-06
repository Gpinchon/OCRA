#include <OCRA/OCRA.hpp>

#include <Common.hpp>

#include <iomanip>

using namespace OCRA;

constexpr auto TEST_COUNT = 1000;

int main(int argc, char const *argv[])
{
    const auto instance = CreateInstance("Test_CommandBuffer");
    const auto physicalDevice = Instance::EnumeratePhysicalDevices(instance).front();
    const auto device = CreateDevice(physicalDevice);
    Descriptor::Pool::Info poolInfo;
    poolInfo.maxSets = 1024;
    poolInfo.sizes.push_back({ Descriptor::Type::UniformBuffer, 1024 });
    auto descriptorPool = Descriptor::Pool::Create(device, poolInfo);
    Descriptor::SetLayout::Info layoutInfo;
    layoutInfo.bindings.push_back({ 0, Descriptor::Type::UniformBuffer, 1 });
    auto descriptorLayout = Descriptor::SetLayout::Create(device, layoutInfo);
    Descriptor::Pool::AllocateInfo allocateInfo;
    allocateInfo.layout = descriptorLayout;
    allocateInfo.pool = descriptorPool;

    double Creating1024Timer = 0;
    double Deleting341Timer = 0;
    double Creating341Timer = 0;
    double UpdatingTimer = 0;
    for (int j = 0; j < TEST_COUNT; ++j)
    {
        std::vector<Descriptor::Set::Handle> descriptorSets(1024);
        {
            Timer timer;
            for (int i = 0; i < 1024; ++i) {
                descriptorSets.at(i) = Descriptor::Pool::AllocateSet(device, allocateInfo);
            }
            Creating1024Timer += timer.Elapsed().count() / double(TEST_COUNT);
        }
        {
            Timer timer;
            for (int i = 0; i < 1024; ++i) {
                if (i % 3 == 0) descriptorSets.at(i).reset();
            }
            Deleting341Timer += timer.Elapsed().count() / double(TEST_COUNT);
        }
        {
            Timer timer;
            for (int i = 0; i < 1024; ++i) {
                if (i % 3 == 0) descriptorSets.at(i) = Descriptor::Pool::AllocateSet(device, allocateInfo);
            }
            Creating341Timer += timer.Elapsed().count() / double(TEST_COUNT);
        }
        {
            Buffer::Info bufferInfo;
            bufferInfo.usage = Buffer::UsageFlagBits::UniformBuffer;
            bufferInfo.size = 1024;
            auto memory = AllocateMemory(physicalDevice, device, 1024, PhysicalDevice::MemoryPropertyFlagBits::DeviceLocal);
            auto buffer = Buffer::Create(device, bufferInfo);
            Buffer::BindMemory(device, buffer, memory, 0);
            std::vector<Descriptor::Set::WriteOperation> writeOperations(1024);
            for (int i = 0; i < 1024; ++i) {
                Descriptor::Set::BufferInfo bufferInfo;
                bufferInfo.buffer = buffer;
                Descriptor::Set::WriteOperation operation;
                operation.dstSet = descriptorSets.at(i);
                operation.type = Descriptor::Type::UniformBuffer;
                operation.bufferInfo = bufferInfo;
                writeOperations.at(i) = operation;
            }
            Timer timer;
            Descriptor::Set::Update(device, writeOperations, {});
            UpdatingTimer += timer.Elapsed().count() / double(TEST_COUNT);
        }
    }
    std::cout << "Creating1024Timer took " << std::setw(8) << unsigned(Creating1024Timer) << " nanoseconds\n";
    std::cout << "Deleting341Timer  took " << std::setw(8) << unsigned(Deleting341Timer) << " nanoseconds\n";
    std::cout << "Creating341Timer  took " << std::setw(8) << unsigned(Creating341Timer) << " nanoseconds\n";
    std::cout << "UpdatingTimer     took " << std::setw(8) << unsigned(UpdatingTimer) << " nanoseconds\n";
    return 0;
}
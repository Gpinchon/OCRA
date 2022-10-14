#pragma once
#include <Common.hpp>
#include <Buffer.hpp>

OCRA_DECLARE_HANDLE(OCRA::PhysicalDevice);
OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Memory);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::Pool);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::Set);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::SetLayout);

/**
* @brief Shader pipeline helpers
*/
template<typename T, size_t Binding = 0>
class UniformBuffer {
public:
    typedef T value_type;
    UniformBuffer(const OCRA::PhysicalDevice::Handle& a_PhysicalDevice, const OCRA::Device::Handle& a_Device)
        : device(a_Device)
    {
        memory = AllocateMemory(a_PhysicalDevice, a_Device, sizeof(value_type), OCRA::PhysicalDevice::MemoryPropertyFlagBits::HostVisible);
        OCRA::Buffer::Info bufferInfo{};
        bufferInfo.size = sizeof(value_type);
        bufferInfo.usage = OCRA::Buffer::UsageFlagBits::UniformBuffer;
        bufferInfo.queueFamilyIndices.push_back(0);
        auto buffer = OCRA::Buffer::Create(a_Device, bufferInfo);
        OCRA::Buffer::BindMemory(device, buffer, memory, 0);
        {
            OCRA::Descriptor::Pool::Info poolInfo{};
            poolInfo.maxSets = 4096;
            poolInfo.sizes = {};
            descriptorPool = OCRA::Descriptor::Pool::Create(device, poolInfo);
        }
        {
            OCRA::Descriptor::SetLayout::Binding binding;
            binding.binding = Binding;
            binding.count = 1;
            binding.type = OCRA::Descriptor::Type::UniformBuffer;
            OCRA::Descriptor::SetLayout::Info info;
            info.bindings.push_back(binding);
            descriptorSetLayout = OCRA::Descriptor::SetLayout::Create(a_Device, info);
        }
        {
            OCRA::Descriptor::Pool::AllocateInfo allocateInfo{};
            allocateInfo.layouts.push_back(descriptorSetLayout);
            allocateInfo.pool = descriptorPool;
            descriptorSet = OCRA::Descriptor::Pool::AllocateSet(device, allocateInfo).front();
        }
        {
            OCRA::Descriptor::Set::BufferInfo info;
            info.buffer = buffer;
            info.offset = 0;
            info.range = sizeof(value_type);
            std::vector<OCRA::Descriptor::Set::WriteOperation> writeOperations(1);
            OCRA::Descriptor::Set::BufferInfo bufferInfo;
            bufferInfo.buffer = buffer;
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(value_type);
            writeOperations.front().bufferInfo = bufferInfo;
            writeOperations.front().dstSet = descriptorSet;
            writeOperations.front().dstBinding = Binding;
            writeOperations.front().type = OCRA::Descriptor::Type::UniformBuffer;
            OCRA::Descriptor::Set::Update(device, writeOperations, {});
        }

    }
    void Update() {
        if (!dirty) return;
        OCRA::Memory::MappedRange range;
        range.length = sizeof(value_type);
        range.memory = memory;
        auto ptr = OCRA::Memory::Map(device, range);
        std::memcpy(ptr, &data, sizeof(value_type));
        OCRA::Memory::Unmap(device, memory);
        dirty = false;
    }
    void Set(const value_type a_Data) {
        data = a_Data;
        dirty = true;
    }
    auto& Get() const {
        return data;
    }
    auto& GetDescriptorSet() const {
        return descriptorSet;
    }
    auto& GetDescriptorSetLayout() const {
        return descriptorSetLayout;
    }
private:
    bool dirty{ true };
    value_type data;
    OCRA::Device::Handle device;
    OCRA::Memory::Handle memory;
    OCRA::Descriptor::Pool::Handle      descriptorPool;
    OCRA::Descriptor::Set::Handle       descriptorSet;
    OCRA::Descriptor::SetLayout::Handle descriptorSetLayout;
};

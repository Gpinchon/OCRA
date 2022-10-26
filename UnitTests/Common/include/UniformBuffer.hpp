#pragma once
#include <Common.hpp>
#include <Buffer.hpp>

#include <Uniform.hpp>

OCRA_DECLARE_HANDLE(OCRA::PhysicalDevice);
OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Memory);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::Pool);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::Set);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::SetLayout);

/**
* @brief Shader pipeline helpers
*/
namespace OCRA {
template<typename T>
class UniformBuffer : public Uniform {
public:
    typedef T value_type;
    UniformBuffer(const uint32_t& a_Binding, const PhysicalDevice::Handle& a_PhysicalDevice, const Device::Handle& a_Device);
    virtual void Update() override;
    void Set(const value_type a_Data) {
        data = a_Data;
        dirtyData = true;
    }
    auto& Get() const { return data; }

private:
    static Descriptor::SetLayout::Binding CreateSetLayoutBinding(const uint32_t a_Binding) {
        Descriptor::SetLayout::Binding binding;
        binding.binding = a_Binding;
        binding.count = 1;
        binding.type = Descriptor::Type::UniformBuffer;
        return binding;
    }
    bool dirtyDescriptorSet{ true };
    bool dirtyData{ true };
    value_type data;
    Buffer::Handle buffer;
    Memory::Handle memory;
};

template<typename T>
inline UniformBuffer<T>::UniformBuffer(const uint32_t& a_Binding, const PhysicalDevice::Handle& a_PhysicalDevice, const Device::Handle& a_Device)
    : Uniform({ CreateSetLayoutBinding(a_Binding) }, a_Device)
{
    memory = AllocateMemory(a_PhysicalDevice, a_Device, sizeof(value_type), PhysicalDevice::MemoryPropertyFlagBits::HostVisible);
    Buffer::Info bufferInfo{};
    bufferInfo.size = sizeof(value_type);
    bufferInfo.usage = Buffer::UsageFlagBits::UniformBuffer;
    bufferInfo.queueFamilyIndices.push_back(0);
    buffer = Buffer::Create(a_Device, bufferInfo);
    Buffer::BindMemory(GetDevice(), buffer, memory, 0);
}

template<typename T>
inline void UniformBuffer<T>::Update() {
    if (dirtyDescriptorSet) {
        Descriptor::Set::BufferInfo info;
        info.buffer = buffer;
        info.offset = 0;
        info.range = sizeof(value_type);
        std::vector<Descriptor::Set::WriteOperation> writeOperations(1);
        Descriptor::Set::BufferInfo bufferInfo;
        bufferInfo.buffer = buffer;
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(value_type);
        writeOperations.front().bufferInfo = bufferInfo;
        writeOperations.front().dstSet = GetDescriptorSets().front();
        writeOperations.front().dstBinding = GetDescriptorSetLayoutBindings().front().binding;
        writeOperations.front().type = Descriptor::Type::UniformBuffer;
        Descriptor::Set::Update(GetDevice(), writeOperations, {});
    }
    if (dirtyData) {
        Memory::MappedRange range;
        range.length = sizeof(value_type);
        range.memory = memory;
        auto ptr = Memory::Map(GetDevice(), range);
        std::memcpy(ptr, &data, sizeof(value_type));
        Memory::Unmap(GetDevice(), memory);
        dirtyData = false;
    }
}
}

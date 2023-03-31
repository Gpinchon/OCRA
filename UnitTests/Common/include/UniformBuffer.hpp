#pragma once
#include <OCRA/OCRA.hpp>

#include <Common.hpp>
#include <Uniform.hpp>

#include <cassert>

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
class UniformBuffer : public Uniform {
public:
    template<typename T>
    UniformBuffer(const Device::Handle& a_Device, const uint32_t& a_Binding, const T& a_DefaultValue);
    virtual void Update() override;
    inline auto GetDataSize() const {
        return data.size();
    }
    template<typename T>
    inline void Set(const T& a_Data) {
        assert(sizeof(T) == GetDataSize());
        std::memcpy(data.data(), &a_Data, sizeof(T));
        dirtyData = true;
    }
    template<typename T>
    inline auto& Get() const {
        assert(sizeof(T) == GetDataSize());
        return *reinterpret_cast<const T*>(data.data());
    }

private:
    inline static DescriptorSetLayoutBinding CreateSetLayoutBinding(const uint32_t a_Binding) {
        DescriptorSetLayoutBinding binding;
        binding.binding = a_Binding;
        binding.count = 1;
        binding.type = DescriptorType::UniformBuffer;
        return binding;
    }
    bool dirtyDescriptorSet{ true };
    bool dirtyData{ true };
    std::vector<std::byte> data;
    Buffer::Handle buffer;
    Memory::Handle memory;
};

template<typename T>
inline UniformBuffer::UniformBuffer(const Device::Handle& a_Device, const uint32_t& a_Binding, const T& a_DefaultValue)
    : Uniform(a_Device, { CreateSetLayoutBinding(a_Binding) })
    , data(sizeof(T))
{
    Set(a_DefaultValue);
    AllocateBufferInfo bufferInfo;
    bufferInfo.bufferFlags = CreateBufferFlagBits::None;
    bufferInfo.bufferUsage = BufferUsageFlagBits::UniformBuffer;
    bufferInfo.memoryFlags = MemoryPropertyFlagBits::HostVisible;
    bufferInfo.queueFamilyIndices = { 0 };
    bufferInfo.sharingMode = SharingMode::Exclusive;
    bufferInfo.size = GetDataSize();
    buffer = Device::AllocateBuffer(a_Device, bufferInfo);
    memory = Buffer::GetMemory(buffer);
}
}

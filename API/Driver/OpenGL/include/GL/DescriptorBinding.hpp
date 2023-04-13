#pragma once

#include <OCRA/Core.hpp>

#include <GL/Common/Assert.hpp>
#include <GL/DescriptorSetLayout.hpp>
#include <GL/Buffer.hpp>
#include <GL/Memory.hpp>
#include <GL/ImageView.hpp>
#include <GL/ImageSampler.hpp>

#include <GL/glew.h>

namespace OCRA::Descriptor
{
struct Storage {
    virtual ~Storage() = default;
    virtual void operator=(const Storage& a_Other) = 0;
    virtual void operator=(const DescriptorSetWrite& a_Write) = 0;
    virtual void Bind(uint32_t a_BindingIndex) = 0;
    virtual void Unbind(uint32_t a_BindingIndex) = 0;
};
struct ImageStorage : Storage {
    virtual ~ImageStorage() override = default;
    virtual void operator=(const Storage& a_Other) override {
        auto& data = static_cast<const ImageStorage&>(a_Other);
        imageView = data.imageView;
    }
    virtual void operator=(const DescriptorSetWrite& a_Write) override {
        imageView = a_Write.imageInfo->imageView;
    }
    virtual void Bind(uint32_t a_BindingIndex) override {
        glActiveTexture(GL_TEXTURE0 + a_BindingIndex);
        glBindTexture(imageView->target, imageView->handle);
    }
    virtual void Unbind(uint32_t a_BindingIndex) override {
        glActiveTexture(GL_TEXTURE0 + a_BindingIndex);
        glBindTexture(imageView->target, 0);
    }
    OCRA::Image::View::Handle     imageView{ nullptr };
    
};
struct ImageSampler : ImageStorage {
    virtual ~ImageSampler() override = default;
    virtual void operator=(const Storage& a_Other) override {
        ImageStorage::operator=(a_Other);
        auto& data = static_cast<const ImageSampler&>(a_Other);
        imageSampler = data.imageSampler;
    }
    virtual void operator=(const DescriptorSetWrite& a_Write) override {
        ImageStorage::operator=(a_Write);
        imageSampler = a_Write.imageInfo->sampler;
    }
    virtual void Bind(uint32_t a_BindingIndex) override {
        ImageStorage::Bind(a_BindingIndex);
        glBindSampler(a_BindingIndex, imageSampler->handle);
    }
    virtual void Unbind(uint32_t a_BindingIndex) override {
        ImageStorage::Unbind(a_BindingIndex);
        glBindSampler(a_BindingIndex, 0);
        /*glBindImageTexture(
            a_BindingIndex,
            0,
            0, //level
            GL_FALSE, //layered
            0, //layer
            GL_READ_WRITE,
            imageView->format);*/
    }
    OCRA::Image::Sampler::Handle  imageSampler{ nullptr };
};
struct StorageImage : ImageStorage {
};

struct BufferStorage : Storage {
    virtual ~BufferStorage() override = default;
    virtual void operator=(const Storage& a_Other) override {
        auto& data = static_cast<const BufferStorage&>(a_Other);
        OCRA_ASSERT(data.range > 0 && "range must be greather than ZERO");
        buffer = data.buffer;
        offset = data.offset;
        range  = data.range;
    }
    virtual void operator=(const DescriptorSetWrite& a_Write) override {
        const auto& bufferInfo = a_Write.bufferInfo.value();
        OCRA_ASSERT(bufferInfo.range > 0 && "range must be greather than ZERO");
        buffer = bufferInfo.buffer;
        offset = bufferInfo.offset + buffer->memoryBinding.offset;
        range  = bufferInfo.range;
    }
    virtual void Bind(uint32_t a_BindingIndex) override {}
    virtual void Unbind(uint32_t a_BindingIndex) override {}
    OCRA::Buffer::Handle buffer{ nullptr };
    size_t               offset{ 0 };
    size_t               range{ 0 };
};
struct UniformTexelBuffer : BufferStorage {
};
struct StorageTexelBuffer : BufferStorage {
};
struct UniformBuffer : BufferStorage {
    virtual void Bind(uint32_t a_BindingIndex) override {
        glBindBufferRange(
            GL_UNIFORM_BUFFER,
            a_BindingIndex,
            buffer->memoryBinding.memory->handle,
            offset, range);
    }
    virtual void Unbind(uint32_t a_BindingIndex) override {
        glBindBufferRange(GL_UNIFORM_BUFFER, a_BindingIndex, 0, 0, 0);
    }
};
struct UniformBufferDynamic : UniformBuffer {};
struct StorageBuffer : BufferStorage {
    virtual void Bind(uint32_t a_BindingIndex) override {
        glBindBufferRange(
            GL_SHADER_STORAGE_BUFFER,
            a_BindingIndex,
            buffer->memoryBinding.memory->handle,
            offset, range);
    }
    virtual void Unbind(uint32_t a_BindingIndex) override {
        glBindBufferRange(GL_SHADER_STORAGE_BUFFER, a_BindingIndex, 0, 0, 0);
    }
};
struct StorageBufferDynamic : StorageBuffer {};

struct InputAttachment : Storage {
    virtual void operator=(const Storage& a_Other) override {
        auto& data = static_cast<const InputAttachment&>(a_Other);
    }
    virtual void operator=(const DescriptorSetWrite& a_Write) override {}
    virtual void Bind(uint32_t a_BindingIndex) override {}
    virtual void Unbind(uint32_t a_BindingIndex) override {}
};
struct InlineUniformBlock : Storage {
    virtual void operator=(const Storage& a_Other) override {
        auto& data = static_cast<const InlineUniformBlock&>(a_Other);
    }
    virtual void operator=(const DescriptorSetWrite& a_Write) override {}
    virtual void Bind(uint32_t a_BindingIndex) override {}
    virtual void Unbind(uint32_t a_BindingIndex) override {}
};
struct AccelerationStructure : Storage {
    virtual void operator=(const Storage& a_Other) override {
        auto& data = static_cast<const AccelerationStructure&>(a_Other);
    }
    virtual void operator=(const DescriptorSetWrite& a_Write) override {}
    virtual void Bind(uint32_t a_BindingIndex) override {}
    virtual void Unbind(uint32_t a_BindingIndex) override {}
};
struct MutableValve : Storage {
    virtual void operator=(const Storage& a_Other) override {
        auto& data = static_cast<const MutableValve&>(a_Other);
    }
    virtual void operator=(const DescriptorSetWrite& a_Write) override {}
    virtual void Bind(uint32_t a_BindingIndex) override {}
    virtual void Unbind(uint32_t a_BindingIndex) override {}
};

class Binding
{
public:
    Binding() = default;
    Binding(const DescriptorType& a_Type, uint32_t a_BindingIndex);
    Binding(Binding&& a_Other) noexcept;
    Binding(const Binding& a_Other) noexcept;
    Binding(const DescriptorSetWrite& a_Write, uint32_t a_BindingIndex);
    ~Binding();

    void Bind();
    void Unbind();

    void operator=(const Binding& a_Other) {
        OCRA_ASSERT(type == a_Other.type);
        bindingIndex = a_Other.bindingIndex;
        *storage = *a_Other.storage;
    }
    void operator=(const DescriptorSetWrite& a_Write) {
        OCRA_ASSERT(type == a_Write.type);
        *storage = a_Write;
    }

    DescriptorType type{ DescriptorType::Unknown };
    uint32_t       bindingIndex{ uint32_t(-1) };
    //this is a "fake" pointer to avoid heap allocation and allow for virtuals
    Storage*       storage{ nullptr };

private:
    //use union as storage because we get aligned memory
    union Memory {
        Memory() {};
        ~Memory() {};
        ImageStorage imageStorage;
        ImageSampler imageSampler;
        StorageImage storageImage;
        BufferStorage bufferStorage;
        UniformTexelBuffer uniformTexelBuffer;
        StorageTexelBuffer storageTexelBuffer;
        UniformBuffer uniformBuffer;
        UniformBufferDynamic uniformBufferDynamic;
        StorageBuffer storageBuffer;
        StorageBufferDynamic storageBufferDynamic;
        InputAttachment inputAttachment;
        InlineUniformBlock inlineUniformBlock;
        AccelerationStructure accelerationStructure;
        MutableValve mutableValve;
    } _memory{};
};

inline Binding::Binding(const DescriptorType& a_Type, uint32_t a_BindingIndex)
    : type(a_Type)
    , bindingIndex(a_BindingIndex)
{
    switch (type)
    {
    case DescriptorType::ImageSampler:
        storage = new(&_memory.imageSampler) ImageSampler;
        break;
    case DescriptorType::StorageImage:
        storage = new(&_memory.storageImage) StorageImage;
        break;
    case DescriptorType::UniformTexelBuffer:
        storage = new(&_memory.uniformTexelBuffer) UniformTexelBuffer;
        break;
    case DescriptorType::StorageTexelBuffer:
        storage = new(&_memory.storageTexelBuffer) StorageTexelBuffer;
        break;
    case DescriptorType::UniformBuffer:
        storage = new(&_memory.uniformBuffer) UniformBuffer;
        break;
    case DescriptorType::StorageBuffer:
        storage = new(&_memory.storageBuffer) StorageBuffer;
        break;
    case DescriptorType::UniformBufferDynamic:
        storage = new(&_memory.uniformBufferDynamic) UniformBufferDynamic;
        break;
    case DescriptorType::StorageBufferDynamic:
        storage = new(&_memory.storageBufferDynamic) StorageBufferDynamic;
        break;
    case DescriptorType::InputAttachment:
        storage = new(&_memory.inputAttachment) InputAttachment;
        break;
    case DescriptorType::InlineUniformBlock:
        storage = new(&_memory.inlineUniformBlock) InlineUniformBlock;
        break;
    case DescriptorType::AccelerationStructure:
        storage = new(&_memory.accelerationStructure) AccelerationStructure;
        break;
    case DescriptorType::MutableValve:
        storage = new(&_memory.mutableValve) MutableValve;
        break;
    }
}

inline Binding::Binding(Binding&& a_Other) noexcept
    : Binding(a_Other.type, a_Other.bindingIndex)
{
    if (a_Other.storage != nullptr) //else we have unknown set data
        *storage = std::move(*a_Other.storage);
}

inline Binding::Binding(const Binding& a_Other) noexcept
    : Binding(a_Other.type, a_Other.bindingIndex)
{
    if (a_Other.storage != nullptr) //else we have unknown set data
        *storage = *a_Other.storage;
}

inline Binding::Binding(const DescriptorSetWrite& a_Write, uint32_t a_BindingIndex)
    : Binding(a_Write.type, a_BindingIndex)
{
    *storage = a_Write;
}

inline Binding::~Binding() {
    if (storage != nullptr)
        std::destroy_at(storage);
}

inline void Binding::Bind() {
    if (storage != nullptr)
        storage->Bind(bindingIndex);
}

inline void Binding::Unbind() {
    if (storage != nullptr)
        storage->Unbind(bindingIndex);
}

}
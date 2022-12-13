#pragma once

#include <OCRA/Descriptor/Type.hpp>

#include <GL/Descriptor/SetLayout.hpp>

#include <GL/Buffer.hpp>
#include <GL/Memory.hpp>

#include <GL/Image/View.hpp>
#include <GL/Image/Sampler.hpp>

#include <cassert>
#include <memory>
#include <GL/glew.h>

namespace OCRA::Descriptor::Set
{
struct Storage {
    virtual void operator=(const Storage& a_Other) = 0;
    virtual void operator=(const WriteOperation& a_Write) = 0;
    virtual void Bind(uint32_t a_BindingIndex) = 0;
    virtual void Unbind(uint32_t a_BindingIndex) = 0;
};
struct ImageStorage : Storage {
    virtual void operator=(const Storage& a_Other) override {
        auto& data = static_cast<const ImageStorage&>(a_Other);
        imageView = data.imageView;
    }
    virtual void operator=(const WriteOperation& a_Write) override {
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
struct SampledImage : ImageStorage {
    virtual void operator=(const Storage& a_Other) override {
        ImageStorage::operator=(a_Other);
        auto& data = static_cast<const SampledImage&>(a_Other);
        imageSampler = data.imageSampler;
    }
    virtual void operator=(const WriteOperation& a_Write) override {
        ImageStorage::operator=(a_Write);
        imageSampler = a_Write.imageInfo->sampler;
    }
    virtual void Bind(uint32_t a_BindingIndex) override {
        ImageStorage::Bind(a_BindingIndex);
        glBindSampler(a_BindingIndex, imageSampler->handle);
        glBindImageTexture(
            a_BindingIndex,
            imageView->handle,
            0, //level
            GL_FALSE, //layered
            0, //layer
            GL_READ_WRITE,
            imageView->format);
    }
    virtual void Unbind(uint32_t a_BindingIndex) override {
        ImageStorage::Unbind(a_BindingIndex);
        glBindSampler(a_BindingIndex, 0);
        glBindImageTexture(
            a_BindingIndex,
            0,
            0, //level
            GL_FALSE, //layered
            0, //layer
            GL_READ_WRITE,
            imageView->format);
    }
    OCRA::Image::Sampler::Handle  imageSampler{ nullptr };
};
struct StorageImage : ImageStorage {
};

struct BufferStorage : Storage {
    virtual void operator=(const Storage& a_Other) override {
        auto& data = static_cast<const BufferStorage&>(a_Other);
        buffer = data.buffer;
        offset = data.offset;
        range  = data.range;
    }
    virtual void operator=(const WriteOperation& a_Write) override {
        buffer = a_Write.bufferInfo->buffer;
        offset = a_Write.bufferInfo->offset + buffer->memoryBinding.offset;
        range = a_Write.bufferInfo->range;
    }
    virtual void Bind(uint32_t a_BindingIndex) override {}
    virtual void Unbind(uint32_t a_BindingIndex) override {}
    OCRA::Buffer::Handle buffer;
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
    virtual void operator=(const WriteOperation& a_Write) override {}
    virtual void Bind(uint32_t a_BindingIndex) override {}
    virtual void Unbind(uint32_t a_BindingIndex) override {}
};
struct InlineUniformBlock : Storage {
    virtual void operator=(const Storage& a_Other) override {
        auto& data = static_cast<const InlineUniformBlock&>(a_Other);
    }
    virtual void operator=(const WriteOperation& a_Write) override {}
    virtual void Bind(uint32_t a_BindingIndex) override {}
    virtual void Unbind(uint32_t a_BindingIndex) override {}
};
struct AccelerationStructure : Storage {
    virtual void operator=(const Storage& a_Other) override {
        auto& data = static_cast<const AccelerationStructure&>(a_Other);
    }
    virtual void operator=(const WriteOperation& a_Write) override {}
    virtual void Bind(uint32_t a_BindingIndex) override {}
    virtual void Unbind(uint32_t a_BindingIndex) override {}
};
struct MutableValve : Storage {
    virtual void operator=(const Storage& a_Other) override {
        auto& data = static_cast<const MutableValve&>(a_Other);
    }
    virtual void operator=(const WriteOperation& a_Write) override {}
    virtual void Bind(uint32_t a_BindingIndex) override {}
    virtual void Unbind(uint32_t a_BindingIndex) override {}
};
struct Data
{
    Data(const Type& a_Type, uint32_t a_BindingIndex)
        : type(a_Type)
        , bindingIndex(a_BindingIndex)
    {
        switch (type)
        {
        case Type::SampledImage:
            storage.reset(new SampledImage);
            break;
        case Type::StorageImage:
            storage.reset(new StorageImage);
            break;
        case Type::UniformTexelBuffer:
            storage.reset(new UniformTexelBuffer);
            break;
        case Type::StorageTexelBuffer:
            storage.reset(new StorageTexelBuffer);
            break;
        case Type::UniformBuffer:
            storage.reset(new UniformBuffer);
            break;
        case Type::StorageBuffer:
            storage.reset(new StorageBuffer);
            break;
        case Type::UniformBufferDynamic:
            storage.reset(new UniformBufferDynamic);
            break;
        case Type::StorageBufferDynamic:
            storage.reset(new StorageBufferDynamic);
            break;
        case Type::InputAttachment:
            storage.reset(new InputAttachment);
            break;
        case Type::InlineUniformBlock:
            storage.reset(new InlineUniformBlock);
            break;
        case Type::AccelerationStructure:
            storage.reset(new AccelerationStructure);
            break;
        case Type::MutableValve:
            storage.reset(new MutableValve);
            break;
        }
    }

    Data(Data&& a_Other) noexcept
        : type(std::move(a_Other.type))
        , bindingIndex(std::move(a_Other.bindingIndex))
        , storage(std::move(a_Other.storage))
    {}

    Data(const Data& a_Other)
        : Data(a_Other.type, a_Other.bindingIndex)
    {
        *storage = *a_Other.storage;
    }

    Data(const WriteOperation& a_Write, uint32_t a_BindingIndex)
        : Data(a_Write.type, a_BindingIndex)
    {
        *storage = a_Write;
    }

    void operator=(const Data& a_Other) {
        assert(type == a_Other.type);
        bindingIndex = a_Other.bindingIndex;
        *storage = *a_Other.storage;
    }
    void operator=(const WriteOperation& a_Write) {
        assert(type == a_Write.type);
        *storage = a_Write;
    }

    void Bind() {
        if (storage != nullptr)
            storage->Bind(bindingIndex);
    }
    void Unbind() {
        if (storage != nullptr)
            storage->Unbind(bindingIndex);
    }

    const Type                  type{ Type::Unknown };
    uint32_t                    bindingIndex{ uint32_t(-1) };
    std::unique_ptr<Storage>    storage{ nullptr };
};
}
#pragma once

#include <Descriptor/Type.hpp>

#include <GL/Descriptor/SetLayout.hpp>

#include <cassert>
#include <memory>

namespace OCRA::Descriptor::Set
{
struct Storage {
    virtual void operator=(const Storage& a_Other) = 0;
    virtual void operator=(const WriteOperation& a_Write) = 0;
};
struct ImageStorage : Storage {
	virtual void operator=(const Storage& a_Other) override {
		*this = static_cast<const ImageStorage&>(a_Other);
	}
	virtual void operator=(const WriteOperation& a_Write) override {
		imageView = std::get<ImageInfo>(a_Write.data).imageView;
		imageSampler = std::get<ImageInfo>(a_Write.data).sampler;
	}
	OCRA::Image::View::Handle     imageView{ nullptr };
	OCRA::Image::Sampler::Handle  imageSampler{ nullptr };
};
struct SampledImage : ImageStorage {
};
struct StorageImage : ImageStorage {
};
struct BufferStorage : Storage {
	virtual void operator=(const Storage& a_Other) override {
		*this = static_cast<const BufferStorage&>(a_Other);
	}
	virtual void operator=(const WriteOperation& a_Write) override {
		offset = std::get<BufferInfo>(a_Write.data).offset;
		range = std::get<BufferInfo>(a_Write.data).range;
	}
	OCRA::Buffer::Handle buffer;
	size_t				 offset{ 0 };
	size_t				 range{ 0 };
};
struct UniformTexelBuffer : BufferStorage {
};
struct StorageTexelBuffer : BufferStorage {
};
struct UniformBuffer : BufferStorage {
};
struct StorageBuffer : BufferStorage {
};
struct UniformBufferDynamic : Storage {
    virtual void operator=(const Storage& a_Other) override {
        *this = static_cast<const UniformBufferDynamic&>(a_Other);
    }
    virtual void operator=(const WriteOperation& a_Write) override {}
};
struct StorageBufferDynamic : Storage {
    virtual void operator=(const Storage& a_Other) override {
        *this = static_cast<const StorageBufferDynamic&>(a_Other);
    }
    virtual void operator=(const WriteOperation& a_Write) override {}
};
struct InputAttachment : Storage {
    virtual void operator=(const Storage& a_Other) override {
        *this = static_cast<const InputAttachment&>(a_Other);
    }
    virtual void operator=(const WriteOperation& a_Write) override {}
};
struct InlineUniformBlock : Storage {
    virtual void operator=(const Storage& a_Other) override {
        *this = static_cast<const InlineUniformBlock&>(a_Other);
    }
    virtual void operator=(const WriteOperation& a_Write) override {}
};
struct AccelerationStructure : Storage {
    virtual void operator=(const Storage& a_Other) override {
        *this = static_cast<const AccelerationStructure&>(a_Other);
    }
    virtual void operator=(const WriteOperation& a_Write) override {}
};
struct MutableValve : Storage {
    virtual void operator=(const Storage& a_Other) override {
        *this = static_cast<const MutableValve&>(a_Other);
    }
    virtual void operator=(const WriteOperation& a_Write) override {}
};
struct Data
{
    Data(const Type& a_Type)
        : type(a_Type)
    {
        switch (type)
        {
        case Type::SampledImage:
            storage = new SampledImage;
            break;
        case Type::StorageImage:
            storage = new StorageImage;
            break;
        case Type::UniformTexelBuffer:
            storage = new UniformTexelBuffer;
            break;
        case Type::StorageTexelBuffer:
            storage = new StorageTexelBuffer;
            break;
        case Type::UniformBuffer:
            storage = new UniformBuffer;
            break;
        case Type::StorageBuffer:
            storage = new StorageBuffer;
            break;
        case Type::UniformBufferDynamic:
            storage = new UniformBufferDynamic;
            break;
        case Type::StorageBufferDynamic:
            storage = new StorageBufferDynamic;
            break;
        case Type::InputAttachment:
            storage = new InputAttachment;
            break;
        case Type::InlineUniformBlock:
            storage = new InlineUniformBlock;
            break;
        case Type::AccelerationStructure:
            storage = new AccelerationStructure;
            break;
        case Type::MutableValve:
            storage = new MutableValve;
            break;
        }
    }
	~Data() {
		delete storage;
	}
	Data& operator=(const Data& a_Other) {
		assert(a_Other.type == type);
		*storage = *a_Other.storage;
	}
	Data& operator=(const WriteOperation& a_Write) {
		assert(a_Write.type == type);
		*storage = a_Write;
	}
    const Type type;
    Storage* storage;
};
}
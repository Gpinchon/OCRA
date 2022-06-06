#pragma once

#include <Descriptor/Type.hpp>

#include <GL/Descriptor/SetLayout.hpp>

#include <vector>

namespace OCRA::Descriptor::Set
{
struct Data
{
	Data(const Type& a_Type) : type(a_Type) {}
	virtual void Write(const WriteOperation& a_Write) = 0;
	virtual void Copy(const CopyOperation& a_Copy) = 0;
	const Type type;
};
struct SampledImage : Data {
	SampledImage(const uint32_t& a_Count) : Data(Type::SampledImage) {}
	virtual void Write(const WriteOperation& a_Write) override {}
	virtual void Copy(const CopyOperation& a_Copy) override {}
};
struct StorageImage : Data {
	StorageImage(const uint32_t& a_Count) : Data(Type::StorageImage) {}
	virtual void Write(const WriteOperation& a_Write) override {}
	virtual void Copy(const CopyOperation& a_Copy) override {}
};
struct UniformTexelBuffer : Data {
	UniformTexelBuffer(const uint32_t& a_Count) : Data(Type::UniformTexelBuffer) {}
	virtual void Write(const WriteOperation& a_Write) override {}
	virtual void Copy(const CopyOperation& a_Copy) override {}
};
struct StorageTexelBuffer : Data {
	StorageTexelBuffer(const uint32_t& a_Count) : Data(Type::StorageTexelBuffer) {}
	virtual void Write(const WriteOperation& a_Write) override {}
	virtual void Copy(const CopyOperation& a_Copy) override {}
};
struct UniformBuffer : Data {
	UniformBuffer(const uint32_t& a_Count) : Data(Type::UniformBuffer) {}
	virtual void Write(const WriteOperation& a_Write) override {}
	virtual void Copy(const CopyOperation& a_Copy) override {}
};
struct StorageBuffer : Data {
	StorageBuffer(const uint32_t& a_Count) : Data(Type::StorageBuffer) {}
	virtual void Write(const WriteOperation& a_Write) override {}
	virtual void Copy(const CopyOperation& a_Copy) override {}
};
struct UniformBufferDynamic : Data {
	UniformBufferDynamic(const uint32_t& a_Count) : Data(Type::UniformBufferDynamic) {}
	virtual void Write(const WriteOperation& a_Write) override {}
	virtual void Copy(const CopyOperation& a_Copy) override {}
};
struct StorageBufferDynamic : Data {
	StorageBufferDynamic(const uint32_t& a_Count) : Data(Type::StorageBufferDynamic) {}
	virtual void Write(const WriteOperation& a_Write) override {}
	virtual void Copy(const CopyOperation& a_Copy) override {}
};
struct InputAttachment : Data {
	InputAttachment(const uint32_t& a_Count) : Data(Type::InputAttachment) {}
	virtual void Write(const WriteOperation& a_Write) override {}
	virtual void Copy(const CopyOperation& a_Copy) override {}
};
struct InlineUniformBlock : Data {
	InlineUniformBlock(const uint32_t& a_Count) : Data(Type::InlineUniformBlock) {}
	virtual void Write(const WriteOperation& a_Write) override {}
	virtual void Copy(const CopyOperation& a_Copy) override {}
};
struct AccelerationStructure : Data {
	AccelerationStructure(const uint32_t& a_Count) : Data(Type::AccelerationStructure) {}
	virtual void Write(const WriteOperation& a_Write) override {}
	virtual void Copy(const CopyOperation& a_Copy) override {}
};
struct MutableValve : Data {
	MutableValve(const uint32_t& a_Count) : Data(Type::MutableValve) {}
	virtual void Write(const WriteOperation& a_Write) override {}
	virtual void Copy(const CopyOperation& a_Copy) override {}
};
static inline auto AllocateData(const SetLayout::Handle& a_Layout)
{
	std::vector<Data*> data;
	data.resize(a_Layout->bindings.size());
	for (const auto& binding : a_Layout->bindings)
	{
		switch (binding.type)
		{
			case Type::SampledImage :
				data.at(binding.binding) = new SampledImage(binding.count);
			break;
			case Type::StorageImage :
				data.at(binding.binding) = new StorageImage(binding.count);
			break;
			case Type::UniformTexelBuffer :
				data.at(binding.binding) = new UniformTexelBuffer(binding.count);
			break;
			case Type::StorageTexelBuffer :
				data.at(binding.binding) = new StorageTexelBuffer(binding.count);
			break;
			case Type::UniformBuffer :
				data.at(binding.binding) = new UniformBuffer(binding.count);
			break;
			case Type::StorageBuffer :
				data.at(binding.binding) = new StorageBuffer(binding.count);
			break;
			case Type::UniformBufferDynamic :
				data.at(binding.binding) = new UniformBufferDynamic(binding.count);
			break;
			case Type::StorageBufferDynamic :
				data.at(binding.binding) = new StorageBufferDynamic(binding.count);
			break;
			case Type::InputAttachment :
				data.at(binding.binding) = new InputAttachment(binding.count);
			break;
			case Type::InlineUniformBlock :
				data.at(binding.binding) = new InlineUniformBlock(binding.count);
			break;
			case Type::AccelerationStructure :
				data.at(binding.binding) = new AccelerationStructure(binding.count);
			break;
			case Type::MutableValve :
				data.at(binding.binding) = new MutableValve(binding.count);
			break;
		}
	}
	return data;
}
}
/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 15:03:29
*/
#pragma once

#include <Handle.hpp>
#include <Scalar.hpp>
#include <Pipeline/VertexInputState.hpp>

#include <limits>
#include <array>
#include <queue>
#include <set>
#include <memory>
#include <stdexcept>

#include <GL/Pipeline/VertexInputState.hpp>
#include <GL/glew.h>

namespace OCRA::Pipeline::VertexInputState {
static inline GLenum GetGLFormatType(const AttributeDescription::Format::Type& a_Type)
{
	switch (a_Type)
	{
	case AttributeDescription::Format::Type::Float32:
		return GL_FLOAT;
	case AttributeDescription::Format::Type::Float16:
		return GL_HALF_FLOAT;
	case AttributeDescription::Format::Type::Int32:
		return GL_INT;
	case AttributeDescription::Format::Type::Uint32:
		return GL_UNSIGNED_INT;
	case AttributeDescription::Format::Type::Int16:
		return GL_SHORT;
	case AttributeDescription::Format::Type::Uint16:
		return GL_UNSIGNED_SHORT;
	case AttributeDescription::Format::Type::Int8:
		return GL_BYTE;
	case AttributeDescription::Format::Type::Uint8:
		return GL_UNSIGNED_BYTE;
	default:
		throw std::runtime_error("Unknown Attribute Format Type");
	}
}
struct VAO
{
	VAO()
	{
		glCreateVertexArrays(1, &handle);
	}
	~VAO()
	{
		glDeleteVertexArrays(1, &handle);
	}
	inline void Reset() {
		info = {};
		for (Uint8 attribIndex = 0; attribIndex < info.MaxAttributes; ++attribIndex)
			glDisableVertexArrayAttrib(
				handle,
				attribIndex);
	}
	inline void Set(const Device::Handle& a_Device, const Info& a_Info) noexcept
	{
		info = info;
		for (Uint8 attribIndex = 0; attribIndex < info.attributeDescriptionCount; ++attribIndex)
		{
			const auto& attribute{ info.attributeDescriptions.at(attribIndex) };
			glEnableVertexArrayAttrib(
				handle,
				attribIndex);
			glVertexArrayAttribFormat(
				handle,
				attribIndex,
				attribute.format.valuesPerVertex,
				GetGLFormatType(attribute.format.type),
				attribute.format.normalized,
				attribute.format.relativeOffset);
			const auto& binding{ info.bindingDescriptions.at(attribute.bindingIndex) };
			glVertexArrayAttribBinding(
				handle,
				attribIndex,
				attribute.bindingIndex);
			glVertexArrayVertexBuffer(
				handle,
				attribute.bindingIndex,
				Vertex::Buffer::GetGLHandle(a_Device, binding.buffer),
				binding.offset,
				binding.stride);
		}
	}
	Info info;
	GLuint handle{ 0 };
	Uint64 refCount{ 0 };
};
static inline bool operator!=(const AttributeDescription::Format& a_lAttribFormat, const AttributeDescription::Format& a_rAttribFormat) noexcept
{
	return a_lAttribFormat.type != a_rAttribFormat.type
		|| a_lAttribFormat.relativeOffset != a_rAttribFormat.relativeOffset
		|| a_lAttribFormat.valuesPerVertex != a_rAttribFormat.valuesPerVertex
		|| a_lAttribFormat.normalized != a_rAttribFormat.normalized;
}
static inline bool operator!=(const AttributeDescription& a_lAttrib, const AttributeDescription& a_rAttrib) noexcept
{
	return a_lAttrib.bindingIndex != a_rAttrib.bindingIndex
		|| a_lAttrib.format != a_rAttrib.format;
}
static inline bool operator!=(const BindingDescription& a_lBinding, const BindingDescription& a_rBinding) noexcept
{
	return a_lBinding.buffer != a_rBinding.buffer
		|| a_lBinding.offset != a_rBinding.offset
		|| a_lBinding.stride != a_rBinding.stride
		|| a_lBinding.inputRate != a_rBinding.inputRate;

}
static inline bool operator!=(const Info& a_lInfo, const Info& a_rInfo) noexcept
{
	return a_lInfo.primitiveRestartIndex != a_rInfo.primitiveRestartIndex
		|| a_lInfo.attributeDescriptionCount != a_rInfo.attributeDescriptionCount
		|| a_lInfo.bindingDescriptionCount != a_rInfo.bindingDescriptionCount
		|| a_lInfo.vertexElements != a_rInfo.vertexElements
		|| [a_lInfo, a_rInfo]() {
		for (auto attribIndex = 0u; attribIndex < a_lInfo.attributeDescriptionCount; ++attribIndex) {
			if (a_lInfo.attributeDescriptions.at(attribIndex) != a_rInfo.attributeDescriptions.at(attribIndex)) return true;
		}
		return false;
		}()
		|| [a_lInfo, a_rInfo]() {
		for (auto bindingIndex = 0u; bindingIndex < a_lInfo.bindingDescriptionCount; ++bindingIndex) {
			if (a_lInfo.bindingDescriptions.at(bindingIndex) != a_rInfo.bindingDescriptions.at(bindingIndex)) return true;
		}
		return false;
		}();
}
static inline bool operator==(const Info& a_lInfo, const Info& a_rInfo) noexcept
{
	return !(a_lInfo != a_rInfo);
}
struct VAOPool {
	typedef Uint16 IndexType;
	static constexpr auto MaxVAOs = std::numeric_limits<IndexType>::max();
	struct Reference
	{
		Reference(VAOPool& a_VAOPool, Int32 a_VAOIndex)
			: vaoPool(a_VAOPool.controlBlock)
			, vaoIndex(a_VAOIndex)
		{
			if (!vaoPool.expired()) (*vaoPool.lock())->Ref(vaoIndex);
		}
		~Reference() {
			if (!vaoPool.expired()) (*vaoPool.lock())->Unref(vaoIndex);
		}
		VAO* Get() const
		{
			if (!vaoPool.expired())
				return (*vaoPool.lock())->Get(vaoIndex);
			return nullptr;
		}
		VAO* operator->() const
		{
			return Get();
		}
		std::weak_ptr<VAOPool*> vaoPool;
		Int32 vaoIndex{ -1 };
	};

	VAOPool()
		: controlBlock(new VAOPool*(this))
	{
		for (auto i = 0u; i < MaxVAOs; ++i)
			freeIndices.push(MaxVAOs);
	}
	Reference FindSimilar(const Info& a_Info) noexcept
	{
		for (auto& vaoIndex : usedIndices)
		{
			if (vaoArray.at(vaoIndex).info == a_Info) return Reference(*this, vaoIndex);
		}
		return Reference(*this, -1);
	}
	Reference FindFree() noexcept
	{
		auto vaoIndex{ freeIndices.front() };
		usedIndices.insert(vaoIndex);
		freeIndices.pop();
		return Reference(*this, vaoIndex);
	}
	void Ref(IndexType a_VAOIndex)
	{
		++vaoArray.at(a_VAOIndex).refCount;
	}
	void Unref(IndexType a_VAOIndex)
	{
		auto& vao{ vaoArray.at(a_VAOIndex) };
		if (vao.refCount > 0) --vao.refCount;
		if (vao.refCount == 0) {
			vao.Reset();
			freeIndices.push(a_VAOIndex);
			usedIndices.erase(a_VAOIndex);
		}
	}
	VAO* Get(IndexType a_VAOIndex)
	{
		return &vaoArray.at(a_VAOIndex);
	}
	std::array<VAO, MaxVAOs> vaoArray;
	std::queue<IndexType> freeIndices;
	std::set<IndexType> usedIndices;
	std::shared_ptr<VAOPool*> controlBlock;
};
}
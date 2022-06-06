#pragma once

#include <Handle.hpp>
#include <Descriptor/Set.hpp>

#include <GL/Descriptor/SetData.hpp>

#include <vector>
#include <cassert>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::SetLayout);

namespace OCRA::Descriptor::Set
{
struct Impl
{
	Impl(const SetLayout::Handle& a_Layout)
		: layout(a_Layout)
		, data(AllocateData(a_Layout))
	{};
	void Write(const WriteOperation& a_Write) {
		data.at(a_Write.dstBinding)->Write(a_Write);
	}
	void Copy(const CopyOperation& a_Copy) {
		//assert(a_Copy.type == layout.bindings.at(a_Copy.dstBinding).type);
		data.at(a_Copy.dstBinding)->Copy(a_Copy);
	}
	const SetLayout::Handle layout;
	const std::vector<Data*> data;
};
}
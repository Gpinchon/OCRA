#pragma once

#include <OCRA/Handle.hpp>
#include <OCRA/Descriptor/Type.hpp>
#include <OCRA/Shader/Stage.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::SetLayout);

namespace OCRA {
struct AllocationCallback;
}

namespace OCRA::Descriptor::SetLayout
{
struct Binding
{
    uint32_t    binding{ 0 }; //number of this entry corresponding to a resource inside shader
    Type        type{ Type::Unknown }; //the type of resource descriptor to use
    uint32_t    count{ 0 }; //the nbr of descriptor, when InlineUniformBlock used, is byte size
    Shader::Stage::StageFlags stageFlags { Shader::Stage::StageFlagBits::None }; //the shader stages that can access this resource, might not be used
};
struct Info
{
    std::vector<Binding> bindings;
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info, const AllocationCallback* a_Allocator = nullptr);
}
#pragma once

#include <Handle.hpp>
#include <Shader/Stage.hpp>

OCRA_DECLARE_HANDLE(OCRA::Descriptor::SetLayout);
OCRA_DECLARE_HANDLE(OCRA::Pipeline::Layout);

namespace OCRA {
struct AllocationCallback;
}

namespace OCRA::Pipeline::Layout
{
struct PushConstantRange {
    Shader::Stage::StageFlags stage{ Shader::Stage::StageFlagBits::None };
    uint32_t    offset{ 0 };
    uint32_t    size{ 0 };
};
struct Info
{
    std::vector<Descriptor::SetLayout::Handle>  setLayouts;
    std::vector<PushConstantRange>              pushConstants; //might be unavailable
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info, const AllocationCallback* a_Allocator = nullptr);
}
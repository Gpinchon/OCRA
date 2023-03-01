#pragma once

#include <OCRA/Core.hpp>

namespace OCRA::Shader::Module
{
struct Impl : std::vector<uint32_t>
{
    Impl(const Device::Handle& a_Device, const CreateShaderModuleInfo& a_Info)
        : std::vector<uint32_t>(a_Info.code)
    {}
};
}
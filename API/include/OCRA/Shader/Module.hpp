#pragma once

#include <OCRA/Handle.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Shader::Module);

namespace OCRA::Shader::Module {
struct Info {
    std::vector<uint32_t> code;
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info);
const Info& GetInfo(const Handle& a_Module);
}
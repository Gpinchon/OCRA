#pragma once

#include <Device.hpp>
#include <Handle.hpp>

#include <vector>

HANDLE(OCRA::Shader::Module);

namespace OCRA::Shader::Module {
struct Info {
    std::vector<std::byte> code;
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info);
const Info& GetInfo(const Handle& a_Module);
}
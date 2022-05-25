/*
* @Author: gpinchon
* @Date:   2021-10-04 20:17:52
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-04 20:24:58
*/
#pragma once

#include <Handle.hpp>
#include <Shader/Module.hpp>

#include <vector>
#include <bitset>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Shader::Stage);

namespace OCRA::Shader::Stage {
using StageFlags = std::bitset<6>;
namespace StageFlagBits {
constexpr StageFlags None        = 0b000000;
constexpr StageFlags Vertex      = 0b100000;
constexpr StageFlags Geometry    = 0b010000;
constexpr StageFlags Fragment    = 0b001000;
constexpr StageFlags Compute     = 0b000100;
constexpr StageFlags TessControl = 0b000010;
constexpr StageFlags TessEval    = 0b000001;
constexpr StageFlags AllGraphics = 0b111000; //Vertex | Geometry | Fragment
constexpr StageFlags All         = 0b111111;
}
struct SpecializationMapEntry {
    uint32_t    constantID;
    uint32_t    offset;
    size_t      size;
};
struct SpecializationInfo {
    std::vector<SpecializationMapEntry> mapEntries;
    std::vector<std::byte> data;
};
struct Info {
    StageFlags stage { StageFlagBits::None }; //must be a single stage
    Module::Handle module; //the spir-v module
    std::string entryPoint { "" }; //entry point name
    SpecializationInfo specializationInfo;
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info);
const Info& GetInfo(const Handle& a_Handle);
}
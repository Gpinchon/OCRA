/*
* @Author: gpinchon
* @Date:   2021-10-04 20:17:52
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-04 20:24:58
*/
#pragma once

#include <Device.hpp>
#include <Handle.hpp>
#include <Shader/Module.hpp>

#include <vector>
#include <bitset>

HANDLE(OCRA::Shader::Stage);

namespace OCRA::Shader::Stage {
typedef std::bitset<6> StageFlag;
constexpr StageFlag None = 0b000000;
constexpr StageFlag Vertex = 0b100000;
constexpr StageFlag Geometry = 0b010000;
constexpr StageFlag Fragment = 0b001000;
constexpr StageFlag Compute = 0b000100;
constexpr StageFlag TessControl = 0b000010;
constexpr StageFlag TessEval = 0b000001;
constexpr StageFlag AllGraphics = 0b111000; //Vertex | Geometry | Fragment
constexpr StageFlag All = 0b111111;
enum class Stage {
    None,
    Vertex,
    Geometry,
    Fragment,
    Compute,
    TessControl,
    TessEval,
    MaxValue
};
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
    Stage stage { Stage::None };
    Module::Handle module;
    std::string name { "" }; //entry point name
    SpecializationInfo specializationInfo;
};
Handle Create(const Device::Handle& a_Device, const Info& a_Info);
}
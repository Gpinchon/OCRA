#pragma once

#include <Handle.hpp>

#include <bitset>
#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Command::Buffer);
OCRA_DECLARE_HANDLE(OCRA::Command::Pool);

namespace OCRA {
struct AllocationCallback;
}

/**
* One muse always make sure Command Pool outlives its Command Buffers
*/
namespace OCRA::Command::Pool
{
using CreateFlags = std::bitset<3>;
namespace CreateFlagBits {
static CreateFlags None      = 0b000;
static CreateFlags Transient = 0b100;
static CreateFlags Reset     = 0b010; //enables command buffers to be reset to their initial state
static CreateFlags Protected = 0b001;
}
struct Info
{
    CreateFlags flags{ CreateFlagBits::None };
    uint32_t    queueFamilyIndex{ 0 };
};
struct AllocateInfo {
    Handle      pool;
    enum class Level {
        Unknown = -1, Primary, Secondary
    }           level{ Level::Unknown };
    uint32_t    count{ 0 };
};
Handle Create(
    const Device::Handle&       a_Device,
    const Info&                 a_Info,
    const AllocationCallback*   a_Allocator = nullptr);
std::vector<Buffer::Handle> AllocateBuffer(const Device::Handle& a_Device, const AllocateInfo& a_Info);
const Info& GetInfo(const Handle& a_CommandPool);
}
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

namespace OCRA::Command::Pool
{
using CommandPoolCreateFlags = std::bitset<3>;
namespace CommandPoolCreateFlagBits {
static CommandPoolCreateFlags None					= 0b000;
static CommandPoolCreateFlags Transient				= 0b100;
static CommandPoolCreateFlags ResetCommandBuffer	= 0b010; //enables command buffers to be reset to their initial state
static CommandPoolCreateFlags Protected				= 0b001;
}
struct Info
{
	CommandPoolCreateFlags	flags{ CommandPoolCreateFlagBits::None };
    uint32_t				queueFamilyIndex{ 0 };
};
struct AllocateInfo {
	Command::Pool::Handle	pool;
	enum class Level {
		Unknown = -1, Primary, Secondary
	} level{ Level::Unknown };
	uint32_t				count{ 0 };
};
Handle Create(
	const Device::Handle&		a_Device,
	const Info&					a_Info,
	const AllocationCallback*	a_Allocator = nullptr);
std::vector<Buffer::Handle> AllocateBuffer(const Device::Handle& a_Device, const AllocateInfo& a_Info);
const Info& GetInfo(const Handle& a_CommandPool);
}
/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/
#pragma once

#include <Handle.hpp>

#include <bitset>
#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Command::Pool);
OCRA_DECLARE_HANDLE(OCRA::Command::Buffer);
OCRA_DECLARE_HANDLE(OCRA::Device);

//Functions declaration
namespace OCRA::Command::Buffer
{
enum class Level {
	Unknown = -1, Primary, Secondary
};
struct AllocateInfo {
	Command::Pool::Handle	commandPool;
	Level					level{ Level::Unknown };
	uint32_t				count{ 0 };
};
//Command Buffer allocation
std::vector<Handle> Allocate(const Device::Handle& a_Device, const AllocateInfo& a_Info);
}

//Command Buffer relative commands
namespace OCRA::Command
{
using CommandBufferUsageFlags = std::bitset<3>;
namespace CommandBufferUsageFlagBits {
	static CommandBufferUsageFlags None = 0b000;
	static CommandBufferUsageFlags OneTimeSubmit = 0b100;
	static CommandBufferUsageFlags RenderPassContinue = 0b010;
	static CommandBufferUsageFlags SimultaneousUse = 0b001;
}
struct CommandBufferBeginInfo
{
	CommandBufferUsageFlags flags{ CommandBufferUsageFlagBits::None };
};
//Begin Command Buffer recording, switching it to Recording state
void BeginCommandBuffer(
	const Buffer::Handle& a_CommandBuffer,
	const CommandBufferBeginInfo& a_BeginInfo);
//End Command Buffer recording, switching it to Executable state
void EndCommandBuffer(const Buffer::Handle& a_CommandBuffer);
//Reset Command Buffer to Initial state
void ResetCommandBuffer(const Buffer::Handle& a_CommandBuffer);
//Add a secondary Command Buffer to execute
void ExecuteCommands(
	const Buffer::Handle& a_CommandBuffer,
	const Buffer::Handle& a_SecondaryCommandBuffer);
}
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

OCRA_DECLARE_HANDLE(OCRA::Command::Buffer);
OCRA_DECLARE_HANDLE(OCRA::Device);

namespace OCRA::Command::Buffer
{
using UsageFlags = std::bitset<3>;
namespace UsageFlagBits {
static UsageFlags None = 0b000;
static UsageFlags OneTimeSubmit = 0b100;
static UsageFlags RenderPassContinue = 0b010;
static UsageFlags SimultaneousUse = 0b001;
};
struct BeginInfo
{
	UsageFlags flags{ UsageFlagBits::None };
};
//Begin Command Buffer recording, switching it to Recording state
void Begin(
	const Handle& a_CommandBuffer,
	const BeginInfo& a_BeginInfo);
//End Command Buffer recording, switching it to Executable state
void End(const Handle& a_CommandBuffer);
//Reset Command Buffer to Initial state
void Reset(const Handle& a_CommandBuffer);
}

namespace OCRA::Command
{
//Add a secondary Command Buffer to execute
void ExecuteCommandBuffer(
	const Handle& a_CommandBuffer,
	const Handle& a_SecondaryCommandBuffer);
}
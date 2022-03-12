/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:26:36
*/
#pragma once

#include <Handle.hpp>

HANDLE(OCRA::Command::Buffer);

//Functions declaration
namespace OCRA::Command::Buffer
{
//Command Buffer creation
Handle Create(const Device::Handle& a_Device);
}

//Command Buffer relative commands
namespace OCRA::Command
{
//Begin Command Buffer recording, swutcgubg it to Recording state
void BeginCommandBuffer(const Handle& a_CommandBuffer);
//End Command Buffer recording, switching it to Executable state
void EndCommandBuffer(const Handle& a_CommandBuffer);
//Reset Command Buffer to Initial state
void ResetCommandBuffer(const Handle& a_CommandBuffer);
//Add a secondary Command Buffer to execute
void ExecuteCommands(
	const Buffer::Handle& a_CommandBuffer,
	const Buffer::Handle& a_SecondaryCommandBuffer);
}
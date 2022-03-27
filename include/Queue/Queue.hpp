/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:27:05
*/
#pragma once

#include <Handle.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Queue);
OCRA_DECLARE_HANDLE(OCRA::Queue::Semaphore);
OCRA_DECLARE_HANDLE(OCRA::Queue::Fence);
OCRA_DECLARE_HANDLE(OCRA::Command::Buffer);

namespace OCRA::Queue
{
struct Info {
	uint32_t                    queueFamilyIndex;
	uint32_t                    queueCount;
	std::vector<float>          queuePriorities;
};
struct SubmitInfo
{
	std::vector<Semaphore::Handle>			waitSemaphores;
	std::vector<Command::Buffer::Handle>	commandBuffers;
	std::vector<Semaphore::Handle>			signalSemaphores;
};
void Submit(
	const Handle& a_Queue,
	std::vector<SubmitInfo> a_SubmitInfos,
	const Fence::Handle& a_Fence);
}
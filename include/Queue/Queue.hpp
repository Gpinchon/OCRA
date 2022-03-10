/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:27:05
*/
#pragma once

#include <Command/Buffer.hpp>
#include <Handle.hpp>
#include <Pipeline/StageFlags.hpp>
#include <Queue/Fence.hpp>
#include <Queue/Semaphore.hpp>

#include <vector>

HANDLE(OCRA::Queue);

namespace OCRA::Queue {
struct SubmitInfo {
	std::vector<Semaphore::SubmitInfo>			waitSemaphoreInfos;
	std::vector<Command::Buffer::SubmitInfo>	commandBufferInfos;
	std::vector<Semaphore::SubmitInfo>			signalSemaphoreInfos;
};
void Submit(const Handle& a_Queue, std::vector<SubmitInfo> a_SubmitInfos, const Fence::Handle& a_Fence);
}
/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:27:05
*/
#pragma once

#include <OCRA/Handle.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::Queue);
OCRA_DECLARE_HANDLE(OCRA::Queue::Semaphore);
OCRA_DECLARE_HANDLE(OCRA::Queue::Fence);
OCRA_DECLARE_HANDLE(OCRA::Command::Buffer);

namespace OCRA::Queue
{
struct Info {
    uint32_t            queueFamilyIndex{ 0 };
    uint32_t            queueCount{ 0 };
    std::vector<float>  queuePriorities;
};
/**
* @brief Describes the Timeline Semaphore values to signal/wait for
*/
struct TimelineSemaphoreSubmitInfo {
    std::vector<uint64_t> waitSemaphoreValues;
    std::vector<uint64_t> signalSemaphoreValues;
};
struct SubmitInfo
{
    TimelineSemaphoreSubmitInfo timelineSemaphoreValues;
    std::vector<Semaphore::Handle> waitSemaphores;
    std::vector<OCRA::Command::Buffer::Handle> commandBuffers;
    std::vector<Semaphore::Handle> signalSemaphores;
};
void Submit(
    const Handle& a_Queue,
    const std::vector<SubmitInfo>& a_SubmitInfos,
    const Fence::Handle& a_Fence = nullptr);
/**
* @brief Waits for the queue to have completed its commands.
* On OpenGL, this will push an empty synchronized command.
*/
void WaitIdle(const Handle& a_Queue);
}
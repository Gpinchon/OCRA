/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:27:05
*/
#pragma once

#include <OCRA/Handles.hpp>

#include <vector>
#include <chrono>

namespace OCRA::Semaphore {
void Signal(
    const Handle& a_Semaphore,
    const uint64_t& a_Value);

void Signal(
    const std::vector<Handle>& a_Semaphores,
    const std::vector<uint64_t>& a_Values);

bool Wait(
    const Handle& a_Semaphore,
    const uint64_t& a_Value,
    const std::chrono::nanoseconds& a_TimeoutNS = IgnoreTimeout);

bool Wait(
    const std::vector<Handle>& a_Semaphores,
    const std::vector<uint64_t>& a_Values,
    const std::chrono::nanoseconds& a_TimeoutNS = IgnoreTimeout);

uint64_t GetCounterValue(const Handle& a_Semaphore);
}
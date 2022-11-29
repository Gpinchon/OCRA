/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-09-26 14:27:05
*/
#pragma once

#include <Handle.hpp>
#include <Pipeline/StageFlags.hpp>

#include <vector>
#include <chrono>

namespace OCRA {
struct AllocationCallback;
}

OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Queue::Semaphore);

namespace OCRA::Queue::Semaphore {
enum class Type {
    Unknown = -1, Binary, Timeline
};
struct Info {
    Type type{ Type::Unknown };
    uint64_t initialValue;
};
Handle Create(
    const Device::Handle& a_Device,
    const Info& a_Info,
    const AllocationCallback* a_Allocator = nullptr);
void Signal(
    const Device::Handle& a_Device,
    const Handle& a_Semaphore,
    const uint64_t& a_Value);
void Signal(
    const Device::Handle& a_Device,
    const std::vector<Handle>& a_Semaphores,
    const std::vector<uint64_t>& a_Values);
bool Wait(
    const Device::Handle& a_Device,
    const Handle& a_Semaphore,
    const uint64_t& a_Value,
    const std::chrono::nanoseconds& a_TimeoutNS);
bool Wait(
    const Device::Handle& a_Device,
    const std::vector<Handle>& a_Semaphores,
    const std::vector<uint64_t>& a_Values,
    const std::chrono::nanoseconds& a_TimeoutNS);
uint64_t GetCounterValue(
    const Device::Handle& a_Device,
    const Handle& a_Semaphore);
}
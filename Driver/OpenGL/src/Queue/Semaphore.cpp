#include <Queue/Semaphore.hpp>
#include <Memory/AllocationCallback.hpp>

#include <semaphore>

namespace OCRA::Memory {
struct AllocationCallback;
}

namespace OCRA::Queue::Semaphore {
struct Impl {
	std::counting_semaphore<1> semaphore;
};
Handle Create(
	const Device::Handle& a_Device,
	const Memory::AllocationCallback* a_Allocator = nullptr);
void Signal(
	const Device::Handle& a_Device,
	const Handle& a_Semaphore,
	const uint64_t& a_Value);
bool Wait(
	const Device::Handle& a_Device,
	std::vector<Handle> a_Semaphores,
	std::vector<uint64_t> a_Values,
	uint64_t a_TimeoutMS);
uint64_t GetCounterValue(
	const Device::Handle& a_Device,
	const Handle& a_Semaphore);
}
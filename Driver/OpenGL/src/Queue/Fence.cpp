#include <Queue/Fence.hpp>
#include <Allocator.hpp>

#include <condition_variable>
#include <chrono>

namespace OCRA::Queue::Fence {
struct Impl {
	inline bool WaitFor(uint64_t a_TimeoutMS) {
		const auto duration = std::chrono::duration(std::chrono::milliseconds(a_TimeoutMS));
		return cv.wait_for(
			std::unique_lock<std::mutex>(mutex),
			duration,
			[this]{ return status == Status::Signaled; });
	}
	inline void Signal() {
		std::unique_lock<std::mutex>(mutex);
		status = Status::Signaled;
		cv.notify_all();
	}
	inline void Reset() {
		std::unique_lock<std::mutex>(mutex);
		status = Status::Unsignaled;
	}
	std::mutex mutex;
	std::condition_variable cv;
	Status status{ Status::Unsignaled };
};
Handle Create(
	const Device::Handle& a_Device,
	const AllocationCallback* a_Allocator)
{
	if (a_Allocator != nullptr) {
		auto ptr = a_Allocator->allocationFunc(
			a_Allocator->userData,
			sizeof(Impl), alignof(Impl),
			AllocationScope::Object);
		return Handle(new(ptr) Impl, Deleter(a_Allocator->userData, a_Allocator->freeFunc));
	}
	else return Handle(new Impl);
}
bool WaitFor(
	const Device::Handle& a_Device,
	const std::vector<Handle>& a_Fences,
	bool a_WaitAll, uint64_t a_TimeoutMS)
{
	bool ret = false;
	for (auto& fence : a_Fences) {
		const auto waitResult = fence->WaitFor(a_TimeoutMS);
		if (a_WaitAll) ret |= waitResult;
		else return waitResult;
	}
	return ret;
}
void Signal(
	const Device::Handle& a_Device,
	const std::vector<Handle>& a_Fences)
{
	for (auto& fence : a_Fences) fence->Signal();
}
void Reset(
	const Device::Handle& a_Device,
	const std::vector<Handle>& a_Fences)
{
	for (auto& fence : a_Fences) fence->Reset();
}
Status GetStatus(
	const Device::Handle& a_Device,
	const Handle& a_Fence)
{
	return a_Fence->status;
}
}
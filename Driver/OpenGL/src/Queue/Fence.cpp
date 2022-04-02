#include <Queue/Fence.hpp>
#include <Allocator.hpp>

#include <GL/glew.h>

#include <condition_variable>
#include <chrono>
#include <atomic>

namespace OCRA::Queue::Fence {
struct Impl {
	~Impl() {
		Reset();
	}
	inline auto GetStatus() {
		const auto syncValue = sync.load(std::memory_order_acquire);
		if (syncValue != nullptr) {
			GLint	status;
			GLsizei size;
			glGetSynciv(syncValue, GL_SYNC_STATUS, sizeof(GLenum), &size, &status);
			if (status == GL_SIGNALED) return Status::Signaled;
		}
		return Status::Unsignaled;
	}
	inline bool WaitFor(const std::chrono::nanoseconds& a_TimeoutNS) {
		const auto waitStart = std::chrono::high_resolution_clock::now();
		auto syncValue = sync.load(std::memory_order_acquire);
		if (syncValue == nullptr) //avoid locking mutex if not necessary
			cv.wait_for(std::unique_lock<std::mutex>(mutex), a_TimeoutNS, [this, &syncValue] { return (syncValue = sync.load(std::memory_order_acquire)) != nullptr; });
		const auto elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - waitStart);
		if (elapsedTime > a_TimeoutNS) return false;
		const auto waitResult = glClientWaitSync(syncValue, 0, (a_TimeoutNS - elapsedTime).count());
		if (waitResult == GL_CONDITION_SATISFIED || waitResult == GL_ALREADY_SIGNALED) return true;
		else return false;
	}
	inline void Signal() {
		if (sync.load(std::memory_order_acquire) == nullptr)
			sync.store(glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0), std::memory_order_release);
		cv.notify_all();
	}
	inline void Reset() {
		glDeleteSync(sync.exchange(nullptr, std::memory_order_acq_rel));
	}
	std::atomic<GLsync> sync{ nullptr };
	std::mutex mutex;
	std::condition_variable cv;
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
	const Handle& a_Fences,
	const std::chrono::nanoseconds& a_TimeoutNS)
{
	return a_Fences->WaitFor(a_TimeoutNS);
}
bool WaitFor(
	const Device::Handle& a_Device,
	const std::vector<Handle>& a_Fences,
	bool a_WaitAll,
	const std::chrono::nanoseconds& a_TimeoutNS)
{
	bool ret = false;
	for (auto& fence : a_Fences) {
		const auto waitResult = fence->WaitFor(a_TimeoutNS);
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
	return a_Fence->GetStatus();
}
}
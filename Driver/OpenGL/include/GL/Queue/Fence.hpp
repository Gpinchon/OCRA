#pragma once

#include <Queue/Fence.hpp>
#include <Common/Timer.hpp>

#include <GL/WeakHandle.hpp>
#include <GL/Device.hpp>
#include <GL/glew.h>

#include <condition_variable>
#include <chrono>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Queue::Fence {
struct Impl {
	Impl(const Device::Handle& a_Device) : device(a_Device) {}
	inline auto GetStatus() {
		std::unique_lock<std::mutex>(mutex);
		return status;
	}
	inline bool WaitFor(const std::chrono::nanoseconds& a_TimeoutNS) {
		return cv.wait_for(std::unique_lock<std::mutex>(mutex), a_TimeoutNS, [this] { return status == Status::Signaled; });
	}
	inline void Signal() {
		std::unique_lock<std::mutex> lock(mutex);
		if (status == Status::Signaled) return;
		const auto sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
		glClientWaitSync(sync, 0, GL_TIMEOUT_IGNORED);
		glDeleteSync(sync);
		status = Status::Signaled;
		lock.unlock();
		cv.notify_all();
	}
	inline void Reset() {
		std::unique_lock<std::mutex>(mutex);
		status = Status::Unsignaled;
	}
	const Device::WeakHandle device;
	Status status{ Status::Unsignaled };
	std::mutex mutex;
	std::condition_variable cv;
};
}
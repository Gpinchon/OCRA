#pragma once

#include <Queue/Fence.hpp>
#include <Common/Timer.hpp>

#include <GL/WeakHandle.hpp>
#include <GL/Device.hpp>
#include <GL/glew.h>

#include <condition_variable>
#include <chrono>
#include <atomic>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Queue::Fence {
struct Impl {
	Impl(const Device::Handle& a_Device) : device(a_Device) {}
	~Impl() {
		device.lock()->PushCommand(0, 0, [sync = sync.load(std::memory_order_acquire)]{
			glDeleteSync(sync);
		}, false);
	}
	inline auto GetStatus() {
		const auto syncValue = sync.load(std::memory_order_acquire);
		if (syncValue != nullptr) {
			GLint	status = 0;
			device.lock()->PushCommand(0, 0, [syncValue, &status]{
				GLsizei size;
				glGetSynciv(syncValue, GL_SYNC_STATUS, sizeof(GLenum), &size, &status);
			}, true);
			if (status == GL_SIGNALED) return Status::Signaled;
		}
		return Status::Unsignaled;
	}
	inline bool WaitFor(const std::chrono::nanoseconds& a_TimeoutNS) {
		const Timer timer;
		auto syncValue = sync.load(std::memory_order_acquire);
		if (syncValue == nullptr) //avoid locking mutex if not necessary
			cv.wait_for(std::unique_lock<std::mutex>(mutex), a_TimeoutNS, [this, &syncValue] { return (syncValue = sync.load(std::memory_order_acquire)) != nullptr; });
		GLenum waitResult = 0;
		device.lock()->PushCommand(0, 0, [&syncValue, &timer, &a_TimeoutNS, &waitResult]{
			const auto elapsed = timer.Elapsed();
			if (elapsed > a_TimeoutNS) return;
			const auto timeout = (a_TimeoutNS - elapsed).count();
			waitResult = glClientWaitSync(syncValue, 0, timeout);
		}, true);
		return (waitResult == GL_CONDITION_SATISFIED || waitResult == GL_ALREADY_SIGNALED);
	}
	inline void Signal() {
		if (sync.load(std::memory_order_acquire) == nullptr)
			sync.store(glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0), std::memory_order_release);
		cv.notify_all();
	}
	inline void Reset() {
		device.lock()->PushCommand(0, 0, [this] {
			glDeleteSync(sync.exchange(nullptr, std::memory_order_acq_rel));
		}, false);
	}
	const Device::WeakHandle device;
	std::atomic<GLsync> sync{ nullptr };
	std::mutex mutex;
	std::condition_variable cv;
};
}
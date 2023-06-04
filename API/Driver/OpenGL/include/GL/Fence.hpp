#pragma once

#include <OCRA/Core.hpp>

#include <GL/Device.hpp>
#include <GL/glew.h>

#include <chrono>
#include <condition_variable>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Device);

namespace OCRA::Fence {
struct Impl {
    Impl(const FenceStatus& a_DefaultStatus)
        : status(a_DefaultStatus)
    {
    }
    inline auto GetStatus()
    {
        std::unique_lock<std::mutex> lock(mutex);
        return status;
    }
    inline bool WaitFor(const std::chrono::nanoseconds& a_TimeoutNS)
    {
        auto lock = std::unique_lock<std::mutex>(mutex);
        return cv.wait_for(lock, a_TimeoutNS, [this] { return status == FenceStatus::Signaled; });
    }
    inline void Signal()
    {
        auto lock = std::unique_lock<std::mutex>(mutex);
        if (status == FenceStatus::Signaled)
            return;
        status = FenceStatus::Signaled;
        lock.unlock();
        cv.notify_all();
    }
    inline void Reset()
    {
        auto lock = std::unique_lock<std::mutex>(mutex);
        status    = FenceStatus::Unsignaled;
    }
    FenceStatus status { FenceStatus::Unsignaled };
    std::mutex mutex;
    std::condition_variable cv;
};
}

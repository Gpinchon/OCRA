#include <OCRA/Core.hpp>

#include <GL/glew.h>

#include <mutex>

namespace OCRA::Semaphore {
struct Impl {
    Impl(const SemaphoreType& a_Type)
        : type(a_Type)
    {
    }
    const SemaphoreType type;
};
/**
 * @brief Binary semaphores are simpler as they're
 * not meant to be called directly by the user
 */
struct Binary : Impl {
    Binary()
        : Impl(SemaphoreType::Binary)
    {
    }
    ~Binary() { }
    void Wait()
    {
        OCRA_ASSERT(glIsSync(sync) && "Semaphore not signaled");
        glWaitSync(sync, 0, GL_TIMEOUT_IGNORED);
        glDeleteSync(sync);
    }
    void Signal()
    {
        OCRA_ASSERT(!glIsSync(sync) && "Semaphore not unsignaled");
        sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    }
    GLsync sync = nullptr; // glIsSync when signaled
};

struct Timeline : Impl {
    Timeline(const uint64_t& a_InitialValue)
        : Impl(SemaphoreType::Timeline)
    {
        count = a_InitialValue;
    }
    void SignalDevice(const uint64_t& a_Value)
    {
        mutex.lock();
        const auto sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
        glClientWaitSync(sync, 0, GL_TIMEOUT_IGNORED);
        glDeleteSync(sync);
        count = a_Value;
        mutex.unlock();
        cv.notify_all();
    }
    void SignalClient(const uint64_t& a_Value)
    {
        mutex.lock();
        count = a_Value;
        mutex.unlock();
        cv.notify_all();
    }
    void WaitDevice(const uint64_t& a_Value)
    {
        auto lock = std::unique_lock<std::mutex>(mutex);
        cv.wait(lock, [this, a_Value] { return count >= a_Value; });
    }
    bool WaitClient(const std::chrono::nanoseconds& a_TimeoutNS, const uint64_t& a_Value)
    {
        auto lock = std::unique_lock<std::mutex>(mutex);
        return cv.wait_for(lock, a_TimeoutNS,
            [this, a_Value] { return count >= a_Value; });
        ;
    }
    auto GetCount()
    {
        std::unique_lock lock(mutex);
        return count;
    }
    int64_t count { 0 };
    std::mutex mutex;
    std::condition_variable cv;
};
}

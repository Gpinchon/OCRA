#include <Handle.hpp>
#include <Queue/Semaphore.hpp>

#include <GL/glew.h>

#include <mutex>
#include <assert.h>

OCRA_DECLARE_HANDLE(OCRA::Queue::Semaphore);

namespace OCRA::Queue::Semaphore {
struct Impl {
    Impl(const Type& a_Type)
        : type(a_Type)
    {}
    const Type type;
};
struct Binary : Impl {
    Binary() : Impl(Type::Binary) {}
    ~Binary() {}
    void Wait() {
        cv.wait(std::unique_lock<std::mutex>(mutex),
                [this] { return count > 0; });
        --count;
    }
    void Signal() {
        mutex.lock();
        const auto sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
        glClientWaitSync(sync, 0, GL_TIMEOUT_IGNORED);
        glDeleteSync(sync);
        ++count;
        mutex.unlock();
        cv.notify_all();
    }
    int64_t count{ 0 };
    std::mutex mutex;
    std::condition_variable cv;
};
struct Timeline : Impl {
    Timeline(const uint64_t& a_InitialValue)
        : Impl(Type::Timeline)
        , value(a_InitialValue)
    {}
    void SignalDevice(const uint64_t& a_Value) {
        mutex.lock();
        assert(a_Value > value);
        const auto sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
        glClientWaitSync(sync, 0, GL_TIMEOUT_IGNORED);
        glDeleteSync(sync);
        value = a_Value;
        mutex.unlock();
        cv.notify_all();
    }
    void SignalClient(const uint64_t& a_Value) {
        mutex.lock();
        assert(a_Value > value);
        value = a_Value;
        mutex.unlock();
        cv.notify_all();
    }
    void WaitDevice(const uint64_t& a_Value) {
        cv.wait(std::unique_lock<std::mutex>(mutex),
            [this, waitValue = a_Value] { return value >= waitValue; });
    }
    bool WaitClient(const std::chrono::nanoseconds& a_TimeoutNS, const uint64_t& a_Value) {
        return cv.wait_for(std::unique_lock<std::mutex>(mutex), a_TimeoutNS,
            [this, waitValue = a_Value] { return value >= waitValue; });;
    }
    auto Value() {
        std::unique_lock<std::mutex>(mutex);
        return value;
    }
    uint64_t value{ 0 };
    std::mutex mutex;
    std::condition_variable cv;
};
}
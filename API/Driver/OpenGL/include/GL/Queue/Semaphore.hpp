#include <OCRA/Handle.hpp>
#include <OCRA/Queue/Semaphore.hpp>

#include <GL/glew.h>

#include <mutex>
#include <assert.h>

OCRA_DECLARE_HANDLE(OCRA::Queue::Semaphore);

namespace OCRA::Queue::Semaphore {
struct Impl {
    Impl(const Type& a_Type)
        : type(a_Type)
    {}
    int64_t Count() {
        std::unique_lock<std::mutex> lock(mutex);
        return count;
    }
    const Type type;
    int64_t count{ 0 };
    std::mutex mutex;
    std::condition_variable cv;
};
struct Binary : Impl {
    Binary() : Impl(Type::Binary) {}
    ~Binary() {}
    void Wait() {
        auto lock = std::unique_lock<std::mutex>(mutex);
        cv.wait(lock, [this] { return count > 0; });
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
    void SignalNoSync() {
        mutex.lock();
        ++count;
        mutex.unlock();
        cv.notify_all();
    }
};
struct Timeline : Impl {
    Timeline(const uint64_t& a_InitialValue)
        : Impl(Type::Timeline)
    {
        count = a_InitialValue;
    }
    void SignalDevice(const uint64_t& a_Value) {
        mutex.lock();
        assert(a_Value > count);
        const auto sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
        glClientWaitSync(sync, 0, GL_TIMEOUT_IGNORED);
        glDeleteSync(sync);
        count = a_Value;
        mutex.unlock();
        cv.notify_all();
    }
    void SignalClient(const uint64_t& a_Value) {
        mutex.lock();
        assert(a_Value > count);
        count = a_Value;
        mutex.unlock();
        cv.notify_all();
    }
    void WaitDevice(const uint64_t& a_Value) {
        auto lock = std::unique_lock<std::mutex>(mutex);
        cv.wait(lock, [this, waitValue = a_Value] { return count >= waitValue; });
    }
    bool WaitClient(const std::chrono::nanoseconds& a_TimeoutNS, const uint64_t& a_Value) {
        auto lock = std::unique_lock<std::mutex>(mutex);
        return cv.wait_for(lock, a_TimeoutNS,
            [this, waitValue = a_Value] { return count >= waitValue; });;
    }
};
}
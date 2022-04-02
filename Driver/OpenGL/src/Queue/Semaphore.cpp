#include <Queue/Semaphore.hpp>

#include <GL/glew.h>

#include <mutex>
#include <assert.h>
#include <future>

namespace OCRA {
struct AllocationCallback;
}

namespace OCRA::Queue::Semaphore {
struct Impl {
    Impl(const Type& a_Type)
        : type(a_Type)
    {}
    const Type type;
    
};
struct Binary : Impl {
    Binary() : Impl(Type::Binary) {}
    ~Binary() {
        if (glIsSync(sync)) glDeleteSync(sync);
    }
    void Wait() {
        cv.wait(std::unique_lock<std::mutex>(mutex),
                [this] { return glIsSync(sync); });
        glWaitSync(sync, 0, GL_TIMEOUT_IGNORED);
    }
    void Signal() {
        sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
        cv.notify_all();
    }
    GLsync sync{ 0 };
    std::mutex mutex;
    std::condition_variable cv;
};
struct Timeline : Impl {
    Timeline() : Impl(Type::Timeline) {
        assert(GLEW_NVX_progress_fence);//required to use Timeline
#ifndef glCreateProgressFenceNVX
        throw std::runtime_error("Extension support incomplete");
#else
        semaphore = glCreateProgressFenceNVX();
#endif // !glCreateProgressFenceNVX
    }
    void Signal(const uint64_t& a_Value) {
        glSignalSemaphoreui64NVX(0, 1, &semaphore, &a_Value);
        value = a_Value;
    }
    void WaitDevice(const uint64_t& a_Value) {
        glWaitSemaphoreui64NVX(0, 1, &semaphore, &a_Value);
    }
    bool WaitClient(const std::chrono::nanoseconds& a_TimeoutNS, const uint64_t& a_Value) {
        std::future<void> future = std::async([this, a_Value] { glClientWaitSemaphoreui64NVX(1, &semaphore, &a_Value); });
        return future.wait_for(a_TimeoutNS) == std::future_status::ready;
    }
    auto Value() {
        glClientWaitSemaphoreui64NVX(1, &semaphore, 0);
        return value;
    }
    GLuint semaphore;
    uint64_t value{ 0 };
};
Handle Create(
    const Device::Handle& a_Device,
    const Type& a_Type,
    const AllocationCallback* a_Allocator)
{
    if (a_Type == Type::Timeline)
        return Handle(new Timeline);
    else if (a_Type == Type::Binary)
        return Handle(new Binary);
    throw std::runtime_error("Unknown semaphore type");
}
void SignalDevice(const Handle& a_Semaphore)
{
    assert(a_Semaphore->type == Type::Binary);
    std::static_pointer_cast<Binary>(a_Semaphore)->Signal();
}
void SignalDevice(const Handle& a_Semaphore, const uint64_t& a_Value)
{
    assert(a_Semaphore->type == Type::Timeline);
    std::static_pointer_cast<Timeline>(a_Semaphore)->Signal(a_Value);
}
void Signal(
    const Device::Handle& a_Device,
    const Handle& a_Semaphore,
    const uint64_t& a_Value)
{
    assert(a_Semaphore->type == Type::Timeline);
    std::static_pointer_cast<Timeline>(a_Semaphore)->Signal(a_Value);
}
void Signal(
    const Device::Handle& a_Device,
    const std::vector<Handle>& a_Semaphores,
    const std::vector<uint64_t>& a_Values)
{
    for (auto semaphoreIndex = 0u; semaphoreIndex < a_Semaphores.size(); ++semaphoreIndex)
        Signal(a_Device, a_Semaphores.at(semaphoreIndex), a_Values.at(semaphoreIndex));
}
void WaitDevice(const Handle& a_Semaphore)
{
    assert(a_Semaphore->type == Type::Binary);
    std::static_pointer_cast<Binary>(a_Semaphore)->Wait();
}
void WaitDevice(const Handle& a_Semaphore, const uint64_t& a_Value)
{
    assert(a_Semaphore->type == Type::Timeline);
    std::static_pointer_cast<Timeline>(a_Semaphore)->WaitDevice(a_Value);
}
bool Wait(
    const Device::Handle& a_Device,
    const Handle& a_Semaphore,
    const uint64_t& a_Value,
    const std::chrono::nanoseconds& a_TimeoutNS)
{
    assert(a_Semaphore->type == Type::Timeline);
    return std::static_pointer_cast<Timeline>(a_Semaphore)->WaitClient(a_TimeoutNS, a_Value);
}
bool Wait(
    const Device::Handle& a_Device,
    const std::vector<Handle>& a_Semaphores,
    const std::vector<uint64_t>& a_Values,
    const std::chrono::nanoseconds& a_TimeoutNS)
{
    std::chrono::nanoseconds timeout = a_TimeoutNS; //make a copy to edit it
    const auto waitStart = std::chrono::system_clock::now();
    for (auto i = 0u; i < a_Semaphores.size(); ++i) {
        const auto waitResult = Wait(a_Device, a_Semaphores.at(i), a_Values.at(i), timeout);
        if (!waitResult) return false;
        const auto waitDuration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - waitStart);
        timeout -= waitDuration;
    }
    return true;
}
const Type& GetType(const Handle& a_Semaphore)
{
    return a_Semaphore->type;
}
uint64_t GetCounterValue(
    const Device::Handle& a_Device,
    const Handle& a_Semaphore)
{
    assert(a_Semaphore->type == Type::Timeline);
    return std::static_pointer_cast<Timeline>(a_Semaphore)->Value();
}
}
#include <Queue/Semaphore.hpp>

#include <mutex>

namespace OCRA {
struct AllocationCallback;
}

namespace OCRA::Queue::Semaphore {
struct Impl
{
    inline auto Value() {
        std::unique_lock lock(mtx);
        return value;
    }
    inline void Signal(const uint64_t& a_Value) {
        std::unique_lock lock(mtx);
        value = a_Value;
        cv.notify_all();
    }
    inline auto Wait(const std::chrono::milliseconds& a_TimeoutMS, const uint64_t& a_Value) {
        std::unique_lock lock(mtx);
        return cv.wait_for(lock, a_TimeoutMS, [this, a_Value] { return value == a_Value; });
    }
    std::mutex mtx;
    std::condition_variable cv;
    uint64_t value;
};
Handle Create(
	const Device::Handle& a_Device,
	const AllocationCallback* a_Allocator)
{
	return Handle(new Impl);
}
void Signal(
    const Device::Handle& a_Device,
    const Handle& a_Semaphore,
    const uint64_t& a_Value)
{
    a_Semaphore->Signal(a_Value);
}
void Signal(
    const Device::Handle& a_Device,
    const std::vector<Handle>& a_Semaphores,
    const std::vector<uint64_t>& a_Values)
{
    for (auto semaphoreIndex = 0u; semaphoreIndex < a_Semaphores.size(); ++semaphoreIndex)
        a_Semaphores.at(semaphoreIndex)->Signal(a_Values.at(semaphoreIndex));
}
bool Wait(
    const Device::Handle& a_Device,
    const std::vector<Handle>& a_Semaphores,
    const std::vector<uint64_t>& a_Values,
    const uint64_t& a_TimeoutMS)
{
    auto timeout = std::chrono::milliseconds(a_TimeoutMS);
    const auto waitStart = std::chrono::system_clock::now();
    for (auto i = 0u; i < a_Semaphores.size(); ++i) {
        const auto& semaphore = a_Semaphores.at(i);
        const auto& value = a_Values.at(i);
        if (!semaphore->Wait(timeout, value)) return false;
        const auto waitDuration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - waitStart);
        timeout -= waitDuration;
    }
    return true;
}
uint64_t GetCounterValue(
    const Device::Handle& a_Device,
    const Handle& a_Semaphore)
{
    return a_Semaphore->Value();
}
}
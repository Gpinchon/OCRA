#include <Handle.hpp>

OCRA_DECLARE_HANDLE(OCRA::Queue::Semaphore);

namespace OCRA::Queue::Semaphore {
enum class Type;
const Type& GetType(const Handle& a_Semaphore);
void SignalDevice(const Handle& a_Semaphore);
void SignalDevice(const Handle& a_Semaphore, const uint64_t& a_Value);
void WaitDevice(const Handle& a_Semaphore, const uint64_t& a_Value);
void WaitDevice(const Handle& a_Semaphore);
}
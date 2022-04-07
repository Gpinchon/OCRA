#include <GL/Command/Pool.hpp>
#include <GL/WeakHandle.hpp>

#include <memory>
#include <set>

namespace OCRA {
struct AllocationCallback;
}

OCRA_DECLARE_HANDLE(OCRA::Command::Buffer);
OCRA_DECLARE_WEAK_HANDLE(OCRA::Command::Pool);

namespace OCRA::Command::Pool
{
struct Impl
{
	Impl(const Device::Handle& a_Device, const Info& a_Info)
		: info(a_Info)
	{}
	auto Allocate(const AllocateInfo::Level& a_Level) {
		const auto ptr = memoryPool.allocate();
		buffers.insert(ptr);
		return new(ptr) Buffer::Impl(Buffer::Level(a_Level));
	}
	void Deallocate(Buffer::Impl* a_Ptr) {
		a_Ptr->~Impl();
		memoryPool.free(a_Ptr);
		buffers.erase(a_Ptr);
	}
	void Reset() {
		for (auto& buffer : buffers)
			buffer->Reset();
	}
	std::set<Buffer::Impl*> buffers;
	MemoryPool<Buffer::Impl, 256> memoryPool;
	const Info info;
};
Handle Create(
	const Device::Handle&		a_Device,
	const Info&					a_Info,
	const AllocationCallback*	a_Allocator)
{
	return Handle(new Impl(a_Device, a_Info));
}
const Info& GetInfo(const Handle& a_CommandPool)
{
	return a_CommandPool->info;
}
std::vector<Buffer::Handle> AllocateBuffer(const Device::Handle& a_Device, const AllocateInfo& a_Info)
{
	std::vector<Buffer::Handle> commandBuffers;
	commandBuffers.reserve(a_Info.count);
	const auto deallocator = [pool = WeakHandle(a_Info.commandPool)](Buffer::Impl* a_Buffer) { pool.lock()->Deallocate(a_Buffer); };
	for (auto i = 0u; i < a_Info.count; ++i) {
		auto buffer{ a_Info.commandPool->Allocate(a_Info.level) };
		commandBuffers.push_back(Buffer::Handle(buffer, deallocator));
	}
	return commandBuffers;
}
void Reset(const Device::Handle& a_Device, const Handle& a_Pool)
{
	a_Pool->Reset();
}
}
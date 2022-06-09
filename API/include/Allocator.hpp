#pragma once

#include <stdlib.h>
#include <functional>

namespace OCRA {
enum class AllocationScope {
	Command, Object, Cache, Device, Instance
};
enum class AllocationType {
	Executable
};
//void*()(void* a_UserData, size_t a_Size, size_t a_Alignment, AllocationScope a_AllocationScope)
typedef std::function<void*(void*, size_t, size_t, AllocationScope)> AllocationFunc;
//void*()(void* a_UserData, void* a_Original, size_t a_Size, size_t a_Alignment, AllocationScope a_AllocationScope)
typedef std::function<void*(void*, void*, size_t, size_t, AllocationScope)> ReallocationFunc;
//void()(void* a_UserData, void* a_Memory)
typedef std::function<void(void*, void*)> FreeFunc;
//void()(void* a_UserData, size_t a_Size, AllocationType a_AllocationType, AllocationScope a_AllocationScope)
typedef std::function<void(void*, size_t, AllocationType, AllocationScope)> InternalAllocationNotification;
//void()(void* a_UserData, size_t a_Size, AllocationType a_AllocationType, AllocationScope a_AllocationScope)
typedef std::function<void(void*, size_t, AllocationType, AllocationScope)> InternalFreeNotification;
/**
 * @brief used for debugging or logging
 */
struct AllocationCallback {
	void*							userData{ nullptr };
	AllocationFunc					allocationFunc;
	ReallocationFunc				reallocationFunc;
	FreeFunc						freeFunc;
	InternalAllocationNotification	internalAllocationFunc;
	InternalFreeNotification		internalFreeFunc;
};
template<typename T, AllocationScope Scope>
struct Allocator {
	typedef size_t size_type;
	typedef T value_type;
	typedef T* pointer ;
	inline Allocator(const AllocationCallback& a_AllocationCallback) : cb(a_AllocationCallback) {}
	inline pointer allocate(size_type a_Count) {
		return reinterpret_cast<pointer>(cb.allocationFunc(cb.userData, sizeof(value_type) * a_Count, alignof(value_type), Scope));
	}
	inline void deallocate(pointer a_Ptr, size_type) {
		cb.freeFunc(cb.userData, a_Ptr);
	}
	inline bool operator==(const Allocator& a_Other) {
		return true;
	}
	inline bool operator!=(const Allocator& a_Other) {
		return !(*this == a_Other);
	}
	const AllocationCallback cb;
};
struct Deleter {
	Deleter(void* a_UserData, const FreeFunc a_FreeFunc)
	: userData(a_UserData)
	, freeFunc(a_FreeFunc) {}
	inline void operator()(void* a_Ptr) {
		freeFunc(userData, a_Ptr);
	}
	void* const userData;
	const FreeFunc freeFunc;
};
}
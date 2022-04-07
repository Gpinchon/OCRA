#pragma once

#include <memory>
#include <array>
#include <cassert>

namespace OCRA {
template<typename T, size_t ArenaSize>
struct MemoryPool {
	using value_type = T;
	struct Item {
		using storage_type = std::byte[sizeof(value_type)];
		inline value_type* GetStorage() { return reinterpret_cast<value_type*>(data); }
		Item* next;
		alignas(value_type) storage_type data;
	};
	struct Arena {
		Arena() {
			for (size_t i = 1; i < ArenaSize; ++i)
				storage[i - 1].next = &storage[i];
			storage[ArenaSize - 1].next = nullptr;
		}
		void SetNext(std::unique_ptr<Arena>&& a_Next) {
			assert(next == nullptr);
			next.reset(a_Next.release());
		}
		std::array<Item, ArenaSize> storage;
		std::unique_ptr<Arena>	next;
	};
	MemoryPool()
		: arena(std::make_unique<Arena>())
		, freeList(&arena->storage.front())
	{}
	value_type* allocate() {
		if (freeList == nullptr) {
			auto newArena = std::make_unique<Arena>();
			newArena->SetNext(std::move(arena));
			arena.reset(newArena.release());
			freeList = &arena->storage.front();
		}
		auto item = freeList;
		freeList = item->next;
		return item->GetStorage();
	}
	void free(value_type* a_Ptr) {
		auto item = reinterpret_cast<Item*>(a_Ptr);
		item->next = freeList;
		freeList = item;
	}
	std::unique_ptr<Arena> arena;
	Item* freeList;
};
}
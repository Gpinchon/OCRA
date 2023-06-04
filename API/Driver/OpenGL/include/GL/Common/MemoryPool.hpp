#pragma once

////////////////////////////////////////////////////////////////////////////////
// Includes
////////////////////////////////////////////////////////////////////////////////
#include <cstddef>
#include <cstdint>

////////////////////////////////////////////////////////////////////////////////
// Class declarations
////////////////////////////////////////////////////////////////////////////////
namespace OCRA {
template <typename Type>
class MemoryPool {
public:
    typedef Type value_type;
    typedef uint32_t size_type;
    typedef ptrdiff_t difference_type;
    static_assert(sizeof(value_type) >= sizeof(size_type));

    template <typename U>
    struct rebind {
        typedef MemoryPool<U> other;
    };
    class Deleter {
    public:
        constexpr Deleter(MemoryPool& a_Pool)
            : _memoryPool(a_Pool)
        {
        }
        constexpr void operator()(Type* const a_Ptr)
        {
            std::destroy_at(a_Ptr);
            _memoryPool.deallocate(a_Ptr);
        }

    private:
        MemoryPool& _memoryPool;
    };

    constexpr MemoryPool(size_type a_MaxSize) noexcept;
    constexpr MemoryPool(MemoryPool&& a_Other) noexcept;
    constexpr MemoryPool(const MemoryPool& a_Other) noexcept
        : MemoryPool()
    {
    }
    template <typename U>
    constexpr MemoryPool(const MemoryPool<U>&) noexcept
        : MemoryPool()
    {
    }
    ~MemoryPool();

    /** @return the pointer to a newly allocated object */
    constexpr value_type* allocate() noexcept;
    /** @brief returns the space of the pointer to the pool */
    constexpr void deallocate(value_type* const a_Ptr) noexcept;

    /** @return true if this pool contains no object */
    constexpr bool empty() const noexcept;
    /** @return the count of objects contained in this pool */
    constexpr size_type count() const noexcept;
    /** @return the number of available slots in this pool */
    constexpr size_type free() const noexcept;
    /** @return the maximum number of object this pool can contain */
    constexpr size_type max_size() const noexcept;

    /** @return a deleter constructed from this pool */
    constexpr Deleter deleter() noexcept;

    constexpr size_type index_from_addr(std::byte* a_Ptr) const noexcept;
    constexpr std::byte* addr_from_index(size_type a_Index) noexcept;
    constexpr const std::byte* addr_from_index(size_type a_Index) const noexcept;

    template <typename U>
    bool operator!=(const MemoryPool<U>& a_Right) { return false; }
    template <typename U>
    bool operator==(const MemoryPool<U>& a_Right) { return !(*this != a_Right); }

private:
    size_type _max_size { 0 };
    size_type _cellNumUsed { 0 };
    size_type _cellNumFree { max_size() };
    alignas(value_type) std::byte* _memory { max_size() > 0 ? new std::byte[_max_size * sizeof(value_type)] : nullptr };
    std::byte* _next { _memory };
};

template <typename Type>
constexpr MemoryPool<Type>::MemoryPool(size_type a_MaxSize) noexcept
    : _max_size(a_MaxSize)
{
    if (max_size() == 0)
        return;
    std::memset(_memory, 0, sizeof(sizeof(value_type) * _max_size));
    *reinterpret_cast<size_type*>(_next) = 0;
}

template <typename Type>
constexpr MemoryPool<Type>::MemoryPool(MemoryPool&& a_Other) noexcept
    : _max_size(std::move(a_Other._max_size))
    , _cellNumUsed(std::move(a_Other._cellNumUsed))
    , _cellNumFree(std::move(a_Other._cellNumFree))
    , _memory(std::move(a_Other._memory))
    , _next(std::move(a_Other._next))
{
}

template <typename Type>
inline MemoryPool<Type>::~MemoryPool()
{
    if (max_size() == 0)
        return;
    delete[] _memory;
    _next = nullptr;
}

template <typename Type>
constexpr auto MemoryPool<Type>::allocate() noexcept -> value_type*
{
    if (_cellNumUsed < max_size()) {
        auto p = (size_type*)addr_from_index(_cellNumUsed);
        *p     = ++_cellNumUsed;
    }
    Type* res = nullptr;
    if (_cellNumFree > 0) {
        res = (Type*)_next;
        if (--_cellNumFree > 0)
            _next = addr_from_index(*(uint32_t*)_next);
        else
            _next = nullptr;
    }
    return res;
}

template <typename Type>
constexpr void MemoryPool<Type>::deallocate(value_type* const a_Ptr) noexcept
{
    if (_next != nullptr) {
        *(size_type*)a_Ptr = index_from_addr(_next);
        _next              = (std::byte*)a_Ptr;
    } else {
        *(size_type*)a_Ptr = max_size();
        _next              = (std::byte*)a_Ptr;
    }
    ++_cellNumFree;
}

template <typename Type>
constexpr bool MemoryPool<Type>::empty() const noexcept
{
    return _cellNumFree == max_size();
}

template <typename Type>
constexpr auto MemoryPool<Type>::index_from_addr(std::byte* a_Ptr) const noexcept -> size_type
{
    return size_type(a_Ptr - _memory) / sizeof(value_type);
}

template <typename Type>
constexpr std::byte* MemoryPool<Type>::addr_from_index(size_type a_Index) noexcept
{
    return _memory + (a_Index * sizeof(value_type));
}

template <typename Type>
constexpr const std::byte* MemoryPool<Type>::addr_from_index(size_type a_Index) const noexcept
{
    return _memory + (a_Index * sizeof(value_type));
}

template <typename Type>
constexpr auto MemoryPool<Type>::deleter() noexcept -> Deleter
{
    return Deleter(*this);
}

template <typename Type>
constexpr auto MemoryPool<Type>::count() const noexcept -> size_type
{
    return max_size() - _cellNumFree;
}

template <typename Type>
constexpr auto MemoryPool<Type>::free() const noexcept -> size_type
{
    return _cellNumFree;
}

template <typename Type>
constexpr auto MemoryPool<Type>::max_size() const noexcept -> size_type
{
    return _max_size;
}
}

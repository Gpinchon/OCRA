/*
* @Author: gpinchon
* @Date:   2021-09-26 00:00:00
* @Last Modified by:   gpinchon
* @Last Modified time: 2021-10-03 22:11:23
*/
#pragma once

#include <Scalar.hpp>

#include <array>
#include <limits>
#include <memory>
#include <queue>
#include <set>
#include <stdexcept>

namespace OCRA {
typedef Uint16 IndexType;
template <typename T, IndexType MaxObjects = std::numeric_limits<IndexType>::max()>
struct ObjectPool {
    struct Reference {
        Reference() = default;
        Reference(ObjectPool& a_Pool, Int32 a_Index)
            : pool(a_Pool.controlBlock)
            , index(a_Index)
        {
            if (!pool.expired() && index >= 0)
                (*pool.lock())->Ref(index);
        }
        Reference(const Reference& a_Other)
        {
            *this = a_Other;
        }
        ~Reference()
        {
            if (!pool.expired() && index >= 0)
                (*pool.lock())->Unref(index);
        }

        T* Get() const
        {
            if (!pool.expired() && index >= 0)
                return &(*pool.lock())->Get(index);
            return nullptr;
        }
        T* operator->() const
        {
            return Get();
        }
        Reference& operator=(const Reference& a_Other)
        {
            if (!pool.expired() && index >= 0)
                (*pool.lock())->UnRef(index);
            pool = a_Other.pool;
            index = a_Other.index;
            if (!pool.expired() && index >= 0)
                (*pool.lock())->Ref(index);
            return *this;
        }
        std::weak_ptr<ObjectPool<T, MaxObjects>*> pool;
        Int32 index{ -1 };
    };

    ObjectPool()
        : controlBlock(new ObjectPool<T, MaxObjects>*(this))
    {
        for (auto i = 0u; i < MaxObjects; ++i)
            freeIndices.push(i);
    }
    inline virtual void ReleaseObject(IndexType a_Index) {
        freeIndices.push(a_Index);
        usedIndices.erase(a_Index);
    }
    inline Reference FindFree() noexcept
    {
        auto vaoIndex { freeIndices.front() };
        usedIndices.insert(vaoIndex);
        freeIndices.pop();
        return Reference(*this, vaoIndex);
    }
    inline void Ref(IndexType a_Index)
    {
        ++objectRef.at(a_Index);
    }
    inline void Unref(IndexType a_Index)
    {
        auto& obj { objectArray.at(a_Index) };
        auto& refCount{ objectRef.at(a_Index) };
        if (refCount > 0)
            --refCount;
		else
			throw std::runtime_error("Attempted to unref released object");
        if (refCount == 0)
            ReleaseObject(a_Index);
    }
    inline T& Get(IndexType a_VAOIndex)
    {
        return objectArray.at(a_VAOIndex);
    }
    std::array<Uint32, MaxObjects> objectRef{ 0 };
    std::array<T, MaxObjects> objectArray;
    std::queue<IndexType> freeIndices;
    std::set<IndexType> usedIndices;
    std::shared_ptr<ObjectPool<T, MaxObjects> *> controlBlock;
};
}
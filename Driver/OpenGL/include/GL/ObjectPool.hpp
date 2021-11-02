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
template <typename T, typename IndexType = Uint16, IndexType MaxObjects = std::numeric_limits<IndexType>::max()>
struct ObjectPool {
    typedef ObjectPool<T, IndexType, MaxObjects> ObjectPoolType;
    typedef IndexType IndexType;
    struct Reference {
        Reference() = default;
        Reference(ObjectPool& a_Pool, Int32 a_Index)
            : pool(a_Pool.controlBlock)
            , index(a_Index)
        {
            if (Valid())
                GetPool()->Ref(index);
        }
        Reference(const Reference& a_Other)
        {
            *this = a_Other;
        }
        ~Reference()
        {
            if (Valid())
                GetPool()->Unref(index);
        }
        bool Valid() const {
            return !pool.expired() && index >= 0;
        }
        T* Get() const
        {
            if (Valid())
                return &GetPool()->Get(index);
            return nullptr;
        }
        T* operator->() const
        {
            return Get();
        }
        Reference& operator=(const Reference& a_Other)
        {
            if (Valid())
                GetPool()->Unref(index);
            pool = a_Other.pool;
            index = a_Other.index;
            if (Valid())
                GetPool()->Ref(index);
            return *this;
        }
        ObjectPool* GetPool() const {
            return *pool.lock();
        }
        std::weak_ptr<ObjectPoolType*> pool;
        Int32 index{ -1 };
    };

    ObjectPool()
        : controlBlock(new ObjectPoolType*(this))
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
    std::shared_ptr<ObjectPoolType *> controlBlock;
};
}
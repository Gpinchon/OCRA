#pragma once

#include <OCRA/OCRA.hpp>

namespace OCRA {
struct DefaultVertex {
    Vec3 pos;
    Vec3 color;
    Vec2 uv;
    static auto GetBindingDescriptions() {
        std::vector<VertexBindingDescription> bindings(1);
        bindings.at(0).binding = 0;
        bindings.at(0).stride = sizeof(DefaultVertex);
        bindings.at(0).inputRate = VertexInputRate::Vertex;
        return bindings;
    }
    static auto GetAttributeDescription() {
        std::vector<VertexAttributeDescription> attribs(3);
        attribs.at(0).binding = 0;
        attribs.at(0).location = 0;
        attribs.at(0).format.size = decltype(pos)::length();
        attribs.at(0).format.normalized = false;
        attribs.at(0).format.type = VertexType::Float32;
        attribs.at(0).offset = offsetof(DefaultVertex, pos);
        attribs.at(1).binding = 0;
        attribs.at(1).location = 1;
        attribs.at(1).format.size = decltype(color)::length();
        attribs.at(1).format.normalized = true;
        attribs.at(1).format.type = VertexType::Float32;
        attribs.at(1).offset = offsetof(DefaultVertex, color);
        attribs.at(2).binding = 0;
        attribs.at(2).location = 2;
        attribs.at(2).format.size = decltype(uv)::length();
        attribs.at(2).format.normalized = false;
        attribs.at(2).format.type = VertexType::Float32;
        attribs.at(2).offset = offsetof(DefaultVertex, uv);
        return attribs;
    }
};

class VertexBuffer {
public:
    template<typename V = DefaultVertex>
    VertexBuffer(const PhysicalDevice::Handle& a_PhysicalDevice, const Device::Handle& a_Device, const std::vector<V> a_Vertices)
        : size(sizeof(V) * a_Vertices.size())
        , vertexSize(sizeof(V))
        , memory(AllocateMemory(a_PhysicalDevice, a_Device))
        , buffer(CreateBuffer(a_Device))
        , bindings(V::GetBindingDescriptions())
        , attribs(V::GetAttributeDescription())
    {
        FillMemory(a_Device, a_Vertices.data());
    }
    template<typename V = DefaultVertex>
    VertexBuffer(const Device::Handle& a_Device, const Memory::Handle& a_Memory, const size_t& a_Offset, const size_t& a_Size)
        : offset(a_Offset)
        , size(a_Size)
        , vertexSize(sizeof(V))
        , memory(a_Memory)
        , buffer(CreateBuffer(a_Device, a_Offset))
        , bindings(V::GetBindingDescriptions())
        , attribs(V::GetAttributeDescription())
    {}
    auto& GetBuffer() const { return buffer; }
    auto& GetMemory() const { return memory; }
    auto& GetOffset() const { return offset; }
    auto& GetSize() const { return size; }
    auto& GetVertexSize() const { return vertexSize; }
    auto GetVertexNbr() const { return GetSize() / GetVertexSize(); }
    auto& GetBindingDescriptions() const { return bindings; };
    auto& GetAttribsDescriptions() const { return attribs; }

private:
    Memory::Handle AllocateMemory(const PhysicalDevice::Handle& a_PhysicalDevice, const Device::Handle& a_Device);
    void FillMemory(const void* a_Data);
    Buffer::Handle CreateBuffer(const Device::Handle& a_Device);
    const size_t offset{ 0 };
    const size_t size{ 0 };
    const size_t vertexSize{ 0 };
    const Memory::Handle memory;
    const Buffer::Handle buffer;
    const std::vector<VertexBindingDescription>   bindings;
    const std::vector<VertexAttributeDescription> attribs;
};
}
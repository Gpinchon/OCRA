#pragma once

#include <Handle.hpp>
#include <Common/Vec3.hpp>
#include <Common/Vec4.hpp>

#include <Texture.hpp>

#include <vector>
#include <memory>

OCRA_DECLARE_HANDLE(OCRA::PhysicalDevice);
OCRA_DECLARE_HANDLE(OCRA::Device);

#include <UniformBuffer.hpp>
#include <UniformTexture.hpp>
#include <VertexBuffer.hpp>

namespace OCRA {
class Material
{
public:
    struct Parameters {
        Vec4 diffuse{ 1, 1, 1, 1 };
        float roughness{ 0.5 };
        float metallic{ 0 };
    };
    Material(const PhysicalDevice::Handle& a_PhysicalDevice,
             const Device::Handle& a_Device,
             const Texture& a_DiffuseTexture = {},
             const Parameters& a_Parameters = {})
        : parameters(0, a_PhysicalDevice, a_Device)
        , diffuseTexture(0, a_Device, a_DiffuseTexture)
    {}

    auto& GetParameters() const {
        return parameters.Get();
    }
    void SetParameters(const Parameters& a_Parameter) {
        parameters.Set(a_Parameter);
    }

    auto& GetDiffuseTexture() const {
        return diffuseTexture;
    }
    void SetDiffuseTexture(const Texture2D& a_Texture) {
        diffuseTexture.SetTexture(a_Texture);
    }

    void Update() {
        parameters.Update();
        diffuseTexture.Update();
    }

private:
    std::vector<Descriptor::SetLayout::Binding> GetBindingLayouts() {
        std::vector<Descriptor::SetLayout::Binding> bindings;
        bindings.emplace_back(parameters.GetDescriptorSetLayoutBindings());
        bindings.emplace_back(diffuseTexture.GetDescriptorSetLayoutBindings());
        return bindings;
    }
    UniformBuffer<Parameters>   parameters;
    UniformTexture              diffuseTexture;
};

template<typename V = DefaultVertex>
class Mesh {
public:
    Mesh(const PhysicalDevice::Handle& a_PhysicalDevice, const Device::Handle& a_Device, const VertexBuffer<V>& a_VertexBuffer, const Material& a_Material)
        : device(a_Device)
        , material(a_Material)
        , vertexBuffer(a_VertexBuffer)
    {}
    void Update() {
        material.Update();
    }

private:
    OCRA::Device::Handle    device;
    bool                    dirty{ true };
    Material                material;
    VertexBuffer<V>          vertexBuffer;
};
}
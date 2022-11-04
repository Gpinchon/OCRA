#pragma once

#include <Common/Vec4.hpp>

#include <Material.hpp>

namespace OCRA {
struct PBRParameters {
    Vec4 albedo{ 1, 1, 1, 1 };
    float roughness{ 0.5 };
    float metallic{ 0 };
};

class PBRMaterial : public Material
{
public:
    PBRMaterial(
        const PhysicalDevice::Handle& a_PhysicalDevice,
        const Device::Handle& a_Device,
        const Descriptor::Pool::Handle a_DescriptorPool
    );

    auto& GetAbledoTexture() const {
        return GetTexture(0);
    }
    void SetAlbedoTexture(const Texture& a_Texture) {
        SetTexture(0, a_Texture);
    }

    auto& GetAlbedo() const {
        return GetParameters<PBRParameters>().albedo;
    }
    void SetAlbedo(const Vec4& a_Albedo) {
        PBRParameters parameters = GetParameters<PBRParameters>();
        parameters.albedo = a_Albedo;
        SetParameters<PBRParameters>(parameters);
    }

    auto& GetMetallic() const {
        return GetParameters<PBRParameters>().metallic;
    }
    void SetMetallic(const float& a_Metallic) {
        PBRParameters parameters = GetParameters<PBRParameters>();
        parameters.metallic = a_Metallic;
        SetParameters<PBRParameters>(parameters);
    }

    auto& GetRoughness() const {
        return GetParameters<PBRParameters>().metallic;
    }
    void SetRoughness(const float& a_Roughness) {
        PBRParameters parameters = GetParameters<PBRParameters>();
        parameters.roughness = a_Roughness;
        SetParameters<PBRParameters>(parameters);
    }
};
}
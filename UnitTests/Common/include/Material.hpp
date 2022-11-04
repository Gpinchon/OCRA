#pragma once

#include <Handle.hpp>

#include <UniformTexture.hpp>
#include <UniformBuffer.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::PhysicalDevice);
OCRA_DECLARE_HANDLE(OCRA::Device);
OCRA_DECLARE_HANDLE(OCRA::Descriptor::Pool);
OCRA_DECLARE_HANDLE(OCRA::Shader::Stage);

namespace OCRA {
class Material
{
public:
    template<typename Parameters>
    Material(const PhysicalDevice::Handle& a_PhysicalDevice,
        const Device::Handle& a_Device,
        const Descriptor::Pool::Handle a_DescriptorPool,
        const Parameters& a_Parameters,
        const std::vector<Texture>& a_Textures = {},
        const Shader::Stage::Handle& a_Shader = {})
        : parameters(0, a_PhysicalDevice, a_Device, a_DescriptorPool, Parameters{})
        , fragmentShader(a_Shader)
    {
        for (auto& texture : a_Textures)
            textures.push_back(UniformTexture(0, a_Device, a_DescriptorPool, texture));
        UpdateDescriptor();
        
    }

    template<typename Parameters>
    auto& GetParameters() const {
        return parameters.Get<Parameters>();
    }
    template<typename Parameters>
    void SetParameters(const Parameters& a_Parameter) {
        parameters.Set(a_Parameter);
    }

    auto& GetTextures() const {
        return textures;
    }
    void SetTextures(const std::vector<Texture>& a_Textures) {
        for (size_t index = 0; index < textures.size(); ++index) {
            textures.at(index).SetTexture(a_Textures.at(index));
        }
        UpdateDescriptor();
    }

    auto& GetTexture(const size_t& a_Index) const {
        return textures.at(a_Index).GetTexture();
    }
    void SetTexture(const size_t& a_Index, const Texture& a_Texture) {
        textures.at(a_Index).SetTexture(a_Texture);
        UpdateDescriptor();
    }

    auto& GetDescriptorSets() const {
        return descriptorSets;
    }
    auto& GetDescriptorSetLayouts() const {
        return descriptorSetLayouts;
    }
    auto& GetDescriptorSetLayoutBindings() const {
        return descriptorSetLayoutBindings;
    }

    auto& GetFragmentShader() const {
        return fragmentShader;
    }

    void Update() {
        parameters.Update();
        for (auto& texture : textures)
            texture.Update();
    }

private:
    void UpdateDescriptor() {
        descriptorSets.clear();
        descriptorSetLayouts.clear();
        descriptorSetLayoutBindings.clear();

        descriptorSets.insert(descriptorSets.end(), parameters.GetDescriptorSets().begin(), parameters.GetDescriptorSets().end());
        for (const auto& texture : textures)
            descriptorSets.insert(descriptorSets.end(), texture.GetDescriptorSets().begin(), texture.GetDescriptorSets().end());

        descriptorSetLayouts.push_back(parameters.GetDescriptorSetLayout());
        for (const auto& texture : textures)
            descriptorSetLayouts.push_back(texture.GetDescriptorSetLayout());

        descriptorSetLayoutBindings.insert(descriptorSetLayoutBindings.end(), parameters.GetDescriptorSetLayoutBindings().begin(), parameters.GetDescriptorSetLayoutBindings().end());
        for (const auto& texture : textures)
            descriptorSetLayoutBindings.insert(descriptorSetLayoutBindings.end(), texture.GetDescriptorSetLayoutBindings().begin(), texture.GetDescriptorSetLayoutBindings().end());
    }
    Shader::Stage::Handle       fragmentShader;
    UniformBuffer               parameters;
    std::vector<UniformTexture> textures;
    std::vector<Descriptor::Set::Handle>        descriptorSets;
    std::vector<Descriptor::SetLayout::Handle>  descriptorSetLayouts;
    std::vector<Descriptor::SetLayout::Binding> descriptorSetLayoutBindings;
};
}
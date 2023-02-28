#pragma once

#include <OCRA/Handle.hpp>

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
        const Parameters& a_Parameters,
        const std::vector<Texture>& a_Textures = {},
        const Shader::Stage::Handle& a_Shader = {})
        : parameters(a_PhysicalDevice, a_Device, 1, Parameters{})
        , fragmentShader(a_Shader)
    {
        uint32_t binding = 1;
        for (auto& texture : a_Textures)
            textures.push_back(UniformTexture(a_Device, ++binding, texture));
        
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
    }

    auto& GetTexture(const size_t& a_Index) const {
        return textures.at(a_Index).GetTexture();
    }
    void SetTexture(const size_t& a_Index, const Texture& a_Texture) {
        textures.at(a_Index).SetTexture(a_Texture);
    }

    auto& GetFragmentShader() const {
        return fragmentShader;
    }
    auto& GetDescriptorLayoutBindings() const {
        return descriptorLayoutBindings;
    }
    auto& GetWriteOperations() const {
        return writeOperations;
    }

    void Update() {
        parameters.Update();
        for (auto& texture : textures)
            texture.Update();
        writeOperations = parameters.GetWriteOperations();
        for (const auto& texture : textures) {
            auto& textureWriteOp = texture.GetWriteOperations();
            writeOperations.insert(writeOperations.end(), textureWriteOp.begin(), textureWriteOp.end());
        }
    }

private:
    Shader::Stage::Handle       fragmentShader;
    UniformBuffer               parameters;
    std::vector<UniformTexture> textures;
    std::vector<DescriptorSetLayoutBinding> descriptorLayoutBindings;
    std::vector<DescriptorSetWrite>  writeOperations;
};
}
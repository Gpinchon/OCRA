#pragma once

#include <OCRA/Handle.hpp>
#include <OCRA/Pipeline/InputAssemblyState.hpp>

#include <OCRA/Pipeline/Graphics.hpp>

#include <Mat4x4.hpp>
#include <Material.hpp>
#include <Texture.hpp>
#include <VertexBuffer.hpp>

#include <vector>

OCRA_DECLARE_HANDLE(OCRA::PhysicalDevice);
OCRA_DECLARE_HANDLE(OCRA::Device);

namespace OCRA {
struct Traverser {
    Pipeline::ViewPortState::Info viewPort{};
};
class Mesh {
public:
    Mesh(
        const PhysicalDevice::Handle& a_PhysicalDevice,
        const Device::Handle& a_Device,
        const VertexBuffer& a_VertexBuffer,
        const Material& a_Material);

    auto& GetProjectionMatrix() const {
        return projectionMatrix.Get<Mat4x4>();
    }
    void SetProjectionMatrix(const Mat4x4& a_ProjectionMatrix) {
        projectionMatrix.Set(a_ProjectionMatrix);
    }

    auto& GetVertexShader() const { return vertexShader; }
    void SetVertexShader(const Shader::Stage::Handle& a_Shader) { vertexShader = a_Shader; }

    auto& GetPipelineLayout() const { return layout; }
    auto& GetVertexBuffer() const { return vertexBuffer; }
    auto& GetMaterial() { return material; }
    auto& GetDescriptorSets() const { return descriptorSets; }
    auto& GetDescriptorSetLayouts() const { return descriptorSetLayouts; }
    auto& GetDescriptorSetLayoutBindings() const { return descriptorSetLayoutBindings; }
    std::vector<Shader::Stage::Handle> GetShaderStages() const {
        return { material.GetFragmentShader(), GetVertexShader() };
    }

    auto GetInputAssembly() const { return inputAssembly; }
    void SetInputAssembly(const Pipeline::InputAssemblyState::Info& a_InputAssembly) {
        inputAssembly = a_InputAssembly;
    }

    void Update() {
        material.Update();
        projectionMatrix.Update();
    }

    void Draw(const Command::Buffer::Handle& a_CommandBuffer);

private:
    OCRA::Device::Handle     device;
    Pipeline::Layout::Handle layout;
    Shader::Stage::Handle    vertexShader;
    Pipeline::InputAssemblyState::Info inputAssembly;
    Material                material;
    VertexBuffer            vertexBuffer;
    UniformBuffer           projectionMatrix;
    std::vector<Descriptor::Set::Handle>        descriptorSets;
    std::vector<Descriptor::SetLayout::Handle>  descriptorSetLayouts;
    std::vector<Descriptor::SetLayout::Binding> descriptorSetLayoutBindings;
};
}
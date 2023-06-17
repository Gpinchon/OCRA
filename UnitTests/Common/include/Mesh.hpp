#pragma once

#include <OCRA/OCRA.hpp>

#include <Mat4x4.hpp>
#include <Material.hpp>
#include <Texture.hpp>
#include <VertexBuffer.hpp>

#include <vector>

namespace OCRA {
struct Traverser {
    PipelineViewPortState viewPort {};
};
class Mesh {
public:
    Mesh(
        const Device::Handle& a_Device,
        const VertexBuffer& a_VertexBuffer,
        const Material& a_Material);

    auto GetDescriptorWrites() const
    {
        std::vector<DescriptorSetWrite> writes;
        writes.reserve(
            projectionMatrix.GetWriteOperations().size() + material.GetWriteOperations().size());
        writes.insert(writes.end(),
            projectionMatrix.GetWriteOperations().begin(),
            projectionMatrix.GetWriteOperations().end());
        writes.insert(writes.end(),
            material.GetWriteOperations().begin(),
            material.GetWriteOperations().end());
        return writes;
    }

    auto& GetProjectionMatrix() const
    {
        return projectionMatrix.Get<Mat4x4>();
    }
    void SetProjectionMatrix(const Mat4x4& a_ProjectionMatrix)
    {
        projectionMatrix.Set(a_ProjectionMatrix);
    }

    auto& GetVertexShader() const { return vertexShader; }
    void SetVertexShader(const PipelineShaderStage& a_Shader) { vertexShader = a_Shader; }

    auto& GetVertexBuffer() const { return vertexBuffer; }
    auto& GetMaterial() { return material; }
    auto& GetDescriptorSets() const { return descriptorSets; }
    auto& GetDescriptorSetLayoutBindings() const
    {
        return descriptorSetLayoutBindings;
    }
    std::vector<PipelineShaderStage> GetShaderStages() const
    {
        return { material.GetFragmentShader(), GetVertexShader() };
    }

    auto GetInputAssembly() const { return inputAssembly; }
    void SetInputAssembly(const PipelineInputAssemblyState& a_InputAssembly)
    {
        inputAssembly = a_InputAssembly;
    }

    void Update()
    {
        material.Update();
        projectionMatrix.Update();
    }

    void Draw(const Command::Buffer::Handle& a_CommandBuffer, const Pipeline::Handle& a_Pipeline);

private:
    OCRA::Device::Handle device;
    PipelineShaderStage vertexShader;
    PipelineInputAssemblyState inputAssembly;
    VertexBuffer vertexBuffer;
    Material material;
    UniformBuffer projectionMatrix;
    std::vector<Descriptor::Set::Handle> descriptorSets;
    std::vector<DescriptorSetBinding> descriptorSetLayoutBindings;
};
}

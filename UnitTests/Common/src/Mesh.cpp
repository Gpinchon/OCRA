#include <Mesh.hpp>

#include <OCRA/ShaderCompiler/Compiler.hpp>
#include <OCRA/ShaderCompiler/Shader.hpp>

namespace OCRA {
auto DefaultVertexShader(const Device::Handle& a_Device) {
    const auto compiler = ShaderCompiler::Create();
    ShaderCompiler::Shader::Info shaderInfo;
    shaderInfo.type = ShaderCompiler::Shader::Type::Vertex;
    shaderInfo.entryPoint = "main";
    shaderInfo.source = {
        "#version 450                                                       \n"
        "layout(binding = 0) uniform Transforms {                           \n"
        "   mat4 matrix;                                                    \n"
        "} transforms;                                                      \n"
        "layout(location = 0) in vec2 inPosition;                           \n"
        "layout(location = 1) in vec3 inColor;                              \n"
        "layout(location = 2) in vec2 inTexCoord;                           \n"
        "layout(location = 0) out vec3 vertColor;                           \n"
        "layout(location = 1) out vec2 vertTexCoord;                        \n"
        "void main() {                                                      \n"
        "   gl_Position = transforms.matrix * vec4(inPosition, 0.0, 1.0);   \n"
        "   vertColor = inColor;                                            \n"
        "   vertTexCoord = inTexCoord;                                      \n"
        "}                                                                  \n"
    };
    const auto vertexShader = ShaderCompiler::Shader::Create(compiler, shaderInfo);
    CreateShaderModuleInfo shaderModuleInfo;
    shaderModuleInfo.code = ShaderCompiler::Shader::Compile(vertexShader);
    const auto shaderModule = CreateShaderModule(a_Device, shaderModuleInfo);
    PipelineShaderStage shaderStageInfo;
    shaderStageInfo.entryPoint = "main";
    shaderStageInfo.stage = ShaderStageFlagBits::Vertex;
    shaderStageInfo.module = shaderModule;
    return shaderStageInfo;
}
Mesh::Mesh(const Device::Handle& a_Device, const VertexBuffer& a_VertexBuffer, const Material& a_Material)
    : device(a_Device)
    , material(a_Material)
    , vertexBuffer(a_VertexBuffer)
    , projectionMatrix(a_Device, 0, ShaderStageFlagBits::All, Mat4x4{})
    , vertexShader(DefaultVertexShader(a_Device))
{
    auto& bindingProjMat = projectionMatrix.GetDescriptorSetLayoutBindings();
    auto& bindingMaterial = material.GetDescriptorLayoutBindings();
    descriptorSetLayoutBindings = bindingProjMat;
    descriptorSetLayoutBindings.insert(
        descriptorSetLayoutBindings.end(),
        bindingMaterial.begin(), bindingMaterial.end());
    AllocateDescriptorSetInfo descriptorSetInfo;
    CreatePipelineLayoutInfo layoutInfo;
    layoutInfo.bindings = descriptorSetLayoutBindings;
    //layout = CreatePipelineLayout(device, layoutInfo);

    inputAssembly.topology = PrimitiveTopology::TriangleList;
    inputAssembly.primitiveRestartEnable = false;
}

void Mesh::Draw(const Command::Buffer::Handle& a_CommandBuffer, const Pipeline::Handle& a_Pipeline)
{
    Command::PushDescriptorSet(a_CommandBuffer, a_Pipeline, projectionMatrix.GetWriteOperations());
    Command::PushDescriptorSet(a_CommandBuffer, a_Pipeline, material.GetWriteOperations());
    Command::SetVertexInput(a_CommandBuffer,
        GetVertexBuffer().GetAttribsDescriptions(),
        GetVertexBuffer().GetBindingDescriptions());
    Command::BindVertexBuffers(a_CommandBuffer, 0, { GetVertexBuffer().GetBuffer() }, { 0 });
    Command::Draw(a_CommandBuffer, GetVertexBuffer().GetVertexNbr(), 1, 0, 0);
}
}
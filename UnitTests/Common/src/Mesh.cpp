#include <Mesh.hpp>

#include <OCRA/Command/Draw.hpp>
#include <OCRA/Pipeline/Layout.hpp>

#include <OCRA/ShaderCompiler/Compiler.hpp>
#include <OCRA/ShaderCompiler/Shader.hpp>

OCRA_DECLARE_WEAK_HANDLE(OCRA::Shader::Stage);

namespace OCRA {
auto DefaultVertexShader(const Device::Handle& a_Device) {
    static Shader::Stage::WeakHandle shaderStage;
    if (!shaderStage.expired()) return shaderStage.lock();
    const auto compiler = ShaderCompiler::Create();
    ShaderCompiler::Shader::Info shaderInfo;
    shaderInfo.type = ShaderCompiler::Shader::Type::Vertex;
    shaderInfo.entryPoint = "main";
    shaderInfo.source = {
        "#version 450                                                       \n"
        "layout(location = 0) in vec2 inPosition;                           \n"
        "layout(location = 1) in vec3 inColor;                              \n"
        "layout(set = 0, binding = 0) uniform Transforms {                  \n"
        "   mat4 matrix;                                                    \n"
        "} transforms;                                                      \n"
        "layout(location = 0) out vec3 vertColor;                           \n"
        "void main() {                                                      \n"
        "   gl_Position = transforms.matrix * vec4(inPosition, 0.0, 1.0);   \n"
        "   vertColor = inColor;                                            \n"
        "}                                                                  \n"
    };
    const auto vertexShader = ShaderCompiler::Shader::Create(compiler, shaderInfo);
    Shader::Module::Info shaderModuleInfo;
    shaderModuleInfo.code = ShaderCompiler::Shader::Compile(vertexShader);
    const auto shaderModule = Shader::Module::Create(a_Device, shaderModuleInfo);
    Shader::Stage::Info shaderStageInfo;
    shaderStageInfo.entryPoint = "main";
    shaderStageInfo.stage = Shader::Stage::StageFlagBits::Vertex;
    shaderStageInfo.module = shaderModule;
    auto shaderStageHandle = Shader::Stage::Create(a_Device, shaderStageInfo);
    shaderStage = shaderStageHandle;
    return shaderStageHandle;
}
Mesh::Mesh(const PhysicalDevice::Handle& a_PhysicalDevice, const Device::Handle& a_Device, const VertexBuffer& a_VertexBuffer, const Material& a_Material)
    : device(a_Device)
    , material(a_Material)
    , vertexBuffer(a_VertexBuffer)
    , projectionMatrix(a_PhysicalDevice, a_Device, 0, Mat4x4{})
    , vertexShader(DefaultVertexShader(a_Device))
{
    Pipeline::Layout::Info layoutInfo;
    layoutInfo.setLayouts = GetDescriptorSetLayouts();
    layout = Pipeline::Layout::Create(device, layoutInfo);

    inputAssembly.topology = Primitive::Topology::TriangleList;
    inputAssembly.primitiveRestartEnable = false;
}

void Mesh::Draw(const Command::Buffer::Handle& a_CommandBuffer) {
    Command::PushDescriptorSet(a_CommandBuffer, Pipeline::BindingPoint::Graphics, layout, 0, projectionMatrix.GetWriteOperations());
    Command::PushDescriptorSet(a_CommandBuffer, Pipeline::BindingPoint::Graphics, layout, 1, material.GetWriteOperations());
    Command::BindVertexBuffers(a_CommandBuffer, 0, { GetVertexBuffer().GetBuffer() }, { 0 });
    Command::Draw(a_CommandBuffer, GetVertexBuffer().GetVertexNbr(), 1, 0, 0);
}
}
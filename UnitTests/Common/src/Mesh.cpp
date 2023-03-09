#include <Mesh.hpp>

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
    CreateShaderStageInfo shaderStageInfo;
    shaderStageInfo.entryPoint = "main";
    shaderStageInfo.stage = ShaderStageFlagBits::Vertex;
    shaderStageInfo.module = shaderModule;
    auto shaderStageHandle = CreateShaderStage(a_Device, shaderStageInfo);
    shaderStage = shaderStageHandle;
    return shaderStageHandle;
}
Mesh::Mesh(const Device::Handle& a_Device, const VertexBuffer& a_VertexBuffer, const Material& a_Material)
    : device(a_Device)
    , material(a_Material)
    , vertexBuffer(a_VertexBuffer)
    , projectionMatrix(a_Device, 0, Mat4x4{})
    , vertexShader(DefaultVertexShader(a_Device))
{
    CreatePipelineLayoutInfo layoutInfo;
    layoutInfo.setLayouts = GetDescriptorSetLayouts();
    layout = CreatePipelineLayout(device, layoutInfo);

    inputAssembly.topology = PrimitiveTopology::TriangleList;
    inputAssembly.primitiveRestartEnable = false;
}

void Mesh::Draw(const Command::Buffer::Handle& a_CommandBuffer) {
    Command::PushDescriptorSet(a_CommandBuffer, PipelineBindingPoint::Graphics, layout, projectionMatrix.GetWriteOperations());
    Command::PushDescriptorSet(a_CommandBuffer, PipelineBindingPoint::Graphics, layout, material.GetWriteOperations());
    Command::BindVertexBuffers(a_CommandBuffer, 0, { GetVertexBuffer().GetBuffer() }, { 0 });
    Command::Draw(a_CommandBuffer, GetVertexBuffer().GetVertexNbr(), 1, 0, 0);
}
}
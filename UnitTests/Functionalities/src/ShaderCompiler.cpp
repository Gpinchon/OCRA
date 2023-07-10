#include <OCRA/ShaderCompiler/Compiler.hpp>

#include <iostream>

#include <gtest/gtest.h>

using namespace OCRA;

void VerifyArray(ShaderCompiler::CompiledShader& a_CompiledShader)
{
    ASSERT_FALSE(a_CompiledShader.SPIRVBinary.empty());
    ASSERT_EQ(a_CompiledShader.uniformBuffers.size(), 2);
    auto& materials = a_CompiledShader.GetUniformBuffers().at(1);
    ASSERT_EQ(materials.name, "uMaterials");
    ASSERT_EQ(materials.type.baseType, ShaderCompiler::ShaderResourceBaseType::Struct);
    auto& materialsStruct = *materials.type.structure;
    ASSERT_EQ(materialsStruct.members.size(), 2);
    auto& intensity = materialsStruct[0];
    auto& data = materialsStruct[1];
    ASSERT_EQ(intensity.name, "intensity");
    ASSERT_EQ(intensity.type.baseType, ShaderCompiler::ShaderResourceBaseType::Float);
    ASSERT_EQ(data.name, "data");
    ASSERT_EQ(data.type.baseType, ShaderCompiler::ShaderResourceBaseType::Struct);
    ASSERT_EQ(data.type.arraySize.size(), 1);
    ASSERT_EQ(data.type.arraySize.at(0), 4);
}

TEST(ShaderCompiler, Array)
{
    auto compiler = ShaderCompiler::Create();
    ShaderCompiler::ShaderInfo shaderInfo;
    shaderInfo.type = ShaderCompiler::ShaderType::Vertex;
    shaderInfo.entryPoint = "array";
    shaderInfo.source = {
        "#version 430                                                     \n"
        "struct LightData {                                               \n"
        "   vec3 position;                                                \n"
        "   vec3 color;                                                   \n"
        "};                                                               \n"
        "struct SpecGlossMaterial {                                       \n"
        "   vec3 albedo;                                                  \n"
        "   float spec;                                                   \n"
        "   float gloss;                                                  \n"
        "};                                                               \n"
        "struct MetRoughMaterial {                                        \n"
        "   vec3  albedo;                                                 \n"
        "   float metallic;                                               \n"
        "   float roughness;                                              \n"
        "};                                                               \n"
        "struct MaterialData {                                            \n"
        "   float alpha;                                                  \n"
        "   SpecGlossMaterial specGloss;                                  \n"
        "   MetRoughMaterial  metRough;                                   \n"
        "};                                                               \n"
        "layout(location = 0) in  vec3 vPosition;                         \n"
        "layout(location = 0) out vec3 vLight;                            \n"
        "layout(location = 100, binding = 32) uniform sampler2D uScreen[10];\n"
        "layout(binding = 0) uniform camera {                             \n"
        "   mat4 viewMatrix;                                              \n"
        "} uCameras[4];                                                   \n"
        "layout(binding = 1) uniform material {                           \n"
        "   vec4 intensity;                                               \n"
        "   MaterialData data[4];                                         \n"
        "} uMaterials;                                                    \n"
        "layout(std430, binding = 0) buffer light {                       \n"
        "   LightData data[];                                             \n"
        "} sLights;                                                       \n"
        "void array() {                                                   \n"
        "   gl_Position = uCameras[0].viewMatrix * vec4(vPosition, 1);    \n"
        "   vLight = vec3(0);                                             \n"
        "   for(int i = 0; i < sLights.data.length(); ++i){               \n"
        "      float dist = distance(vPosition, sLights.data[i].position);\n"
        "      vLight += sLights.data[i].color * dist;                    \n"
        "   }                                                             \n"
        "}                                                                \n"
        "void main() {                                                    \n"
        "   array();                                                      \n"
        "}                                                                \n"
    };
    shaderInfo.targetAPI = ShaderCompiler::TargetAPI::OpenGL;
    VerifyArray(ShaderCompiler::Compile(compiler, shaderInfo));
    shaderInfo.targetAPI = ShaderCompiler::TargetAPI::Vulkan;
    VerifyArray(ShaderCompiler::Compile(compiler, shaderInfo));    
}

TEST(ShaderCompiler, EmptyShader)
{
    auto compiler = ShaderCompiler::Create();
    ShaderCompiler::ShaderInfo shaderInfo;
    shaderInfo.type       = ShaderCompiler::ShaderType::Fragment;
    shaderInfo.entryPoint = "emptyShader";
    shaderInfo.source     = {
        "#version 430                   \n"
            "void emptyShader() {}          \n"
            "void main() { emptyShader(); } \n"
    };
    shaderInfo.targetAPI = ShaderCompiler::TargetAPI::OpenGL;
    ASSERT_FALSE(ShaderCompiler::Compile(compiler, shaderInfo).SPIRVBinary.empty());
    shaderInfo.targetAPI = ShaderCompiler::TargetAPI::Vulkan;
    ASSERT_FALSE(ShaderCompiler::Compile(compiler, shaderInfo).SPIRVBinary.empty());
}

TEST(ShaderCompiler, VertexIndex)
{
    auto compiler = ShaderCompiler::Create();
    ShaderCompiler::ShaderInfo shaderInfo;
    shaderInfo.type       = ShaderCompiler::ShaderType::Vertex;
    shaderInfo.entryPoint = "vertexIndex";
    shaderInfo.source     = {
        "#version 430                                           \n"
            "const vec2 positions[3] = vec2[](                      \n"
            "   vec2( 0.0, -0.5),                                   \n"
            "   vec2( 0.5, -0.5),                                   \n"
            "   vec2(-0.5,  0.5)                                    \n"
            ");                                                     \n"
            "void vertexIndex() {                                   \n"
            "   gl_Position = vec4(positions[gl_VertexIndex], 0, 1);\n"
            "}                                                      \n"
            "void main() {                                          \n"
            "   vertexIndex();                                      \n"
            "}                                                      \n"
    };
    {
        shaderInfo.targetAPI = ShaderCompiler::TargetAPI::OpenGL;
        auto compiledShader  = ShaderCompiler::Compile(compiler, shaderInfo);
        ASSERT_FALSE(compiledShader.SPIRVBinary.empty());
    }
    {
        shaderInfo.targetAPI = ShaderCompiler::TargetAPI::Vulkan;
        auto compiledShader  = ShaderCompiler::Compile(compiler, shaderInfo);
        ASSERT_FALSE(compiledShader.SPIRVBinary.empty());
    }
}

void VerifyPushConstant(ShaderCompiler::CompiledShader& a_CompiledShader)
{
    ASSERT_FALSE(a_CompiledShader.SPIRVBinary.empty());
    ASSERT_EQ(a_CompiledShader.pushConstants.size(), 1);
    ASSERT_EQ(a_CompiledShader.GetPushConstants().at(0).name, "PushConstants");
    ASSERT_EQ(a_CompiledShader.GetPushConstants().at(0).type.baseType, OCRA::ShaderCompiler::ShaderResourceBaseType::Struct);
    ASSERT_EQ(a_CompiledShader.GetPushConstants().at(0).type.structure->members.at(0).name, "viewMatrix");
    ASSERT_EQ(a_CompiledShader.stageInputs.size(), 1);
    ASSERT_EQ(a_CompiledShader.GetStageInputs().at(0).name, "vPosition");
}

TEST(ShaderCompiler, PushConstant)
{
    int ret       = 0;
    auto compiler = ShaderCompiler::Create();
    ShaderCompiler::ShaderInfo shaderInfo;
    shaderInfo.type       = ShaderCompiler::ShaderType::Vertex;
    shaderInfo.entryPoint = "pushConstant";
    shaderInfo.source     = {
        "#version 430                                                   \n"
            "layout(location = 0) in vec3 vPosition;                        \n"
            "layout(push_constant) uniform constants {                      \n"
            "   mat4 viewMatrix;                                            \n"
            "} PushConstants;                                               \n"
            "void pushConstant() {                                          \n"
            "   gl_Position = PushConstants.viewMatrix * vec4(vPosition, 1);\n"
            "}                                                              \n"
            "void main() {                                                  \n"
            "   pushConstant();                                             \n"
            "}                                                              \n"
    };
    shaderInfo.targetAPI = ShaderCompiler::TargetAPI::OpenGL;
    VerifyPushConstant(ShaderCompiler::Compile(compiler, shaderInfo));
    shaderInfo.targetAPI = ShaderCompiler::TargetAPI::Vulkan;
    VerifyPushConstant(ShaderCompiler::Compile(compiler, shaderInfo));
}

TEST(ShaderCompiler, InstanceIndex)
{
    auto compiler = ShaderCompiler::Create();
    ShaderCompiler::ShaderInfo shaderInfo;
    shaderInfo.type       = ShaderCompiler::ShaderType::Vertex;
    shaderInfo.entryPoint = "instanceIndex";
    shaderInfo.source     = {
        "#version 430                                                   \n"
            "const vec2 positions0[3] = vec2[](                             \n"
            "   vec2( 0.0, -0.5),                                           \n"
            "   vec2( 0.5, -0.5),                                           \n"
            "   vec2(-0.5,  0.5)                                            \n"
            ");                                                             \n"
            "const vec2 positions1[3] = vec2[](                             \n"
            "   vec2( 0.00, -0.25),                                         \n"
            "   vec2( 0.25, -0.25),                                         \n"
            "   vec2(-0.25,  0.25)                                          \n"
            ");                                                             \n"
            "void instanceIndex() {                                         \n"
            "   if (gl_InstanceIndex == 0)                                  \n"
            "       gl_Position = vec4(positions0[gl_InstanceIndex], 0, 1); \n"
            "   else                                                        \n"
            "       gl_Position = vec4(positions1[gl_InstanceIndex], 0, 1); \n"
            "}                                                              \n"
            "void main() {                                                  \n"
            "   instanceIndex();                                            \n"
            "}                                                              \n"
    };
    {
        shaderInfo.targetAPI = ShaderCompiler::TargetAPI::OpenGL;
        auto compiledShader  = ShaderCompiler::Compile(compiler, shaderInfo);
        ASSERT_FALSE(compiledShader.SPIRVBinary.empty());
    }
    {
        shaderInfo.targetAPI = ShaderCompiler::TargetAPI::Vulkan;
        auto compiledShader  = ShaderCompiler::Compile(compiler, shaderInfo);
        ASSERT_FALSE(compiledShader.SPIRVBinary.empty());
    }
}

TEST(ShaderCompiler, SubpassShader)
{
    auto compiler = ShaderCompiler::Create();
    try {
        ShaderCompiler::ShaderInfo shaderInfo;
        shaderInfo.type       = ShaderCompiler::ShaderType::Fragment;
        shaderInfo.entryPoint = "subpassShader";
        shaderInfo.source     = {
            "#version 430                                                                      \n"
                "layout(binding = 0, input_attachment_index = 0) uniform subpassInput uLastColor;  \n"
                "layout(location = 0) out vec4 color;                                              \n"
                "void subpassShader() {                                                            \n"
                "   color = subpassLoad(uLastColor);                                               \n"
                "}                                                                                 \n"
                "void main() { subpassShader(); }                                                  \n"
        };
        {
            shaderInfo.targetAPI = ShaderCompiler::TargetAPI::OpenGL;
            auto compiledShader  = ShaderCompiler::Compile(compiler, shaderInfo);
            ASSERT_TRUE(compiledShader.SPIRVBinary.empty());
        }
        {
            shaderInfo.targetAPI = ShaderCompiler::TargetAPI::Vulkan;
            auto compiledShader  = ShaderCompiler::Compile(compiler, shaderInfo);
            ASSERT_TRUE(compiledShader.SPIRVBinary.empty());
        }
    } catch (std::exception& e) {
        SUCCEED() << "This should fail unless subpasses are supported";
        SUCCEED() << e.what();
    }
}

TEST(ShaderCompiler, MultipleSet)
{
    auto compiler = ShaderCompiler::Create();
    try {
        ShaderCompiler::ShaderInfo shaderInfo;
        shaderInfo.type       = ShaderCompiler::ShaderType::Fragment;
        shaderInfo.entryPoint = "multipleSet";
        shaderInfo.source     = {
            "#version 430                                                                      \n"
                "layout(binding = 0, input_attachment_index = 0) uniform subpassInput uLastColor;  \n"
                "layout(binding = 1, input_attachment_index = 1) uniform subpassInput uLastDepth;  \n"
                "layout(location = 0) out vec4 color;                                              \n"
                "void multipleSet() {                                                              \n"
                "   color = subpassLoad(uLastColor);                                               \n"
                "   gl_FragDepth = subpassLoad(uLastDepth).r;                                      \n"
                "}                                                                                 \n"
                "void main() { multipleSet(); }                                                    \n"
        };
        {
            shaderInfo.targetAPI = ShaderCompiler::TargetAPI::OpenGL;
            auto compiledShader  = ShaderCompiler::Compile(compiler, shaderInfo);
            ASSERT_FALSE(compiledShader.SPIRVBinary.empty());
        }
        {
            shaderInfo.targetAPI = ShaderCompiler::TargetAPI::Vulkan;
            auto compiledShader  = ShaderCompiler::Compile(compiler, shaderInfo);
            ASSERT_FALSE(compiledShader.SPIRVBinary.empty());
        }
    } catch (std::exception& e) {
        SUCCEED() << "This should fail unless multiple descriptor sets are supported";
        SUCCEED() << e.what();
    }
}

void VerifyUniformLocation(ShaderCompiler::CompiledShader& a_CompiledShader)
{
    ASSERT_FALSE(a_CompiledShader.SPIRVBinary.empty());
    ASSERT_EQ(a_CompiledShader.stageInputs.size(), 1);
    ASSERT_EQ(a_CompiledShader.GetStageInputs().at(0).name, "vPosition");
    ASSERT_EQ(a_CompiledShader.sampledImages.size(), 1);
    ASSERT_EQ(a_CompiledShader.GetSampledImages().at(0).name, "uTexture");
    ASSERT_EQ(a_CompiledShader.uniformBuffers.size(), 1);
    ASSERT_EQ(a_CompiledShader.GetUniformBuffers().at(0).name, "uCamera");
    ASSERT_EQ(a_CompiledShader.GetUniformBuffers().at(0).type.baseType, OCRA::ShaderCompiler::ShaderResourceBaseType::Struct);
    ASSERT_EQ(a_CompiledShader.GetUniformBuffers().at(0).type.structure->members.at(0).name, "viewMatrix");
}

TEST(ShaderCompiler, UniformLocation)
{
    auto compiler = ShaderCompiler::Create();
    ShaderCompiler::ShaderInfo shaderInfo;
    shaderInfo.type       = ShaderCompiler::ShaderType::Vertex;
    shaderInfo.entryPoint = "uniformLocation";
    shaderInfo.source     = {
        "#version 430                                                   \n"
            "layout(location = 0) in vec3 vPosition;                    \n"
            "layout(binding = 0) uniform sampler2D uTexture;            \n"
            "layout(binding = 0) uniform camera {                       \n"
            "   mat4 viewMatrix;                                        \n"
            "} uCamera;                                                 \n"
            "void uniformLocation() {                                   \n"
            "   gl_Position = uCamera.viewMatrix * vec4(vPosition, 1);  \n"
            "}                                                          \n"
            "void main() {                                              \n"
            "   uniformLocation();                                      \n"
            "}                                                          \n"
    };
    shaderInfo.targetAPI = ShaderCompiler::TargetAPI::OpenGL;
    VerifyUniformLocation(ShaderCompiler::Compile(compiler, shaderInfo));
    shaderInfo.targetAPI = ShaderCompiler::TargetAPI::Vulkan;
    VerifyUniformLocation(ShaderCompiler::Compile(compiler, shaderInfo));
}

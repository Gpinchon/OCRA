#include <Common.hpp>
#include <Window.hpp>
#include <UniformBuffer.hpp>
#include <VertexBuffer.hpp>

#include <Instance.hpp>
#include <Device.hpp>
#include <FrameBuffer.hpp>
#include <Memory.hpp>
#include <Buffer.hpp>
#include <RenderPass.hpp>
#include <Surface.hpp>
#include <SwapChain.hpp>
#include <Image/Image.hpp>
#include <Image/View.hpp>
#include <Command/Pool.hpp>
#include <Command/Buffer.hpp>
#include <Command/Draw.hpp>
#include <Command/Scissor.hpp>
#include <Command/ViewPort.hpp>
#include <Pipeline/Layout.hpp>
#include <Descriptor/Pool.hpp>
#include <Descriptor/Set.hpp>
#include <Descriptor/SetLayout.hpp>
#include <Queue/Fence.hpp>
#include <Common/Vec3.hpp>
#include <Common/Vec_Math.hpp>

#include <ShaderCompiler/Compiler.hpp>
#include <ShaderCompiler/Shader.hpp>

#include <Windows.h>

#include <Pipeline/Graphics.hpp>

using namespace OCRA;
constexpr auto VSync = false;

namespace OCRA {
template <size_t C, size_t R, typename T>
struct Mat;
template <typename T>
struct Mat<4, 4, T> {
    typedef T value_type;
    typedef Mat<4, 4, T> type;
    typedef Vec<4, T> col_type;
    typedef Vec<4, T> row_type;

    Mat() : value{
        col_type(1, 0, 0, 0),
        col_type(0, 1, 0, 0),
        col_type(0, 0, 1, 0),
        col_type(0, 0, 0, 1) }
    {}

    template<typename V1, typename V2, typename V3, typename V4>
    Mat(col_type const& a_V1,
        col_type const& a_V2,
        col_type const& a_V3,
        col_type const& a_V4)
    {
        value[0] = a_V1;
        value[1] = a_V2;
        value[2] = a_V3;
        value[3] = a_V4;
    }
    auto& operator[](size_t const& a_Col) {
        return value[a_Col];
    }
    auto& operator[](size_t const& a_Col) const {
        return value[a_Col];
    }
    template<typename T>
    static auto Rotate(Mat<4, 4, T> const& a_M, T a_Angle, Vec<3, T> const& a_V)
    {
        T const a = a_Angle;
        T const c = cos(a);
        T const s = sin(a);

        auto axis(normalize(a_V));
        auto temp((T(1) - c) * axis);

        Mat<4, 4, T> Rotate;
        Rotate[0][0] = c + temp[0] * axis[0];
        Rotate[0][1] = temp[0] * axis[1] + s * axis[2];
        Rotate[0][2] = temp[0] * axis[2] - s * axis[1];

        Rotate[1][0] = temp[1] * axis[0] - s * axis[2];
        Rotate[1][1] = c + temp[1] * axis[1];
        Rotate[1][2] = temp[1] * axis[2] + s * axis[0];

        Rotate[2][0] = temp[2] * axis[0] + s * axis[1];
        Rotate[2][1] = temp[2] * axis[1] - s * axis[0];
        Rotate[2][2] = c + temp[2] * axis[2];

        Mat<4, 4, T> Result;
        Result[0] = a_M[0] * Rotate[0][0] + a_M[1] * Rotate[0][1] + a_M[2] * Rotate[0][2];
        Result[1] = a_M[0] * Rotate[1][0] + a_M[1] * Rotate[1][1] + a_M[2] * Rotate[1][2];
        Result[2] = a_M[0] * Rotate[2][0] + a_M[1] * Rotate[2][1] + a_M[2] * Rotate[2][2];
        Result[3] = a_M[3];
        return Result;
    }

private:
    col_type value[4];
};
typedef Mat<4, 4, float> Mat4x4;
}

Vec3 HSVtoRGB(float fH, float fS, float fV) {
    float fC = fV * fS; // Chroma
    float fHPrime = fmod(fH / 60.0, 6);
    float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
    float fM = fV - fC;
    float fR, fG, fB;

    if (0 <= fHPrime && fHPrime < 1) {
        fR = fC;
        fG = fX;
        fB = 0;
    }
    else if (1 <= fHPrime && fHPrime < 2) {
        fR = fX;
        fG = fC;
        fB = 0;
    }
    else if (2 <= fHPrime && fHPrime < 3) {
        fR = 0;
        fG = fC;
        fB = fX;
    }
    else if (3 <= fHPrime && fHPrime < 4) {
        fR = 0;
        fG = fX;
        fB = fC;
    }
    else if (4 <= fHPrime && fHPrime < 5) {
        fR = fX;
        fG = 0;
        fB = fC;
    }
    else if (5 <= fHPrime && fHPrime < 6) {
        fR = fC;
        fG = 0;
        fB = fX;
    }
    else {
        fR = 0;
        fG = 0;
        fB = 0;
    }

    fR += fM;
    fG += fM;
    fB += fM;
    return { fR, fG, fB };
}

struct PushConstants {
    Vec3 color;
};

struct Vertex {
    Vec2 pos;
    Vec3 color;
    static auto GetBindingDescriptions() {
        std::vector<Pipeline::VertexInputState::BindingDescription> bindings(1);
        bindings.at(0).binding = 0;
        bindings.at(0).stride = sizeof(Vertex);
        bindings.at(0).inputRate = Pipeline::VertexInputState::BindingDescription::InputRate::Vertex;
        return bindings;
    }
    static auto GetAttributeDescription() {
        std::vector<Pipeline::VertexInputState::AttributeDescription> attribs(2);
        attribs.at(0).binding = 0;
        attribs.at(0).location = 0;
        attribs.at(0).format.size = 2;
        attribs.at(0).format.normalized = false;
        attribs.at(0).format.type = VertexType::Float32;
        attribs.at(0).offset = offsetof(Vertex, pos);
        attribs.at(1).binding = 0;
        attribs.at(1).location = 1;
        attribs.at(1).format.size = 3;
        attribs.at(1).format.normalized = true;
        attribs.at(1).format.type = VertexType::Float32;
        attribs.at(1).offset = offsetof(Vertex, color);
        return attribs;
    }
};

struct GraphicsPipelineTestApp : TestApp
{
    GraphicsPipelineTestApp()
        : TestApp("Test_GraphicsPipeline")
        , physicalDevice(Instance::EnumeratePhysicalDevices(instance).front())
        , device(CreateDevice(physicalDevice))
        , window(Window(instance, physicalDevice, device, name, 1280, 720))
    {
        window.OnResize = [this](const Window&, const uint32_t a_Width, const uint32_t a_Height) {
            render = true;
            if (!window.IsClosing()) {
                CreateFrameBuffer();
                CreateGraphicsPipeline();
            }
        };
        window.OnMaximize = window.OnResize;
        window.OnRestore = window.OnResize;
        window.OnMinimize = [this](const Window&, const uint32_t, const uint32_t) { render = false; };
        CreateShaderStages();
        const auto queueFamily = FindQueueFamily(physicalDevice, PhysicalDevice::QueueFlagsBits::Graphics);
        queue = Device::GetQueue(device, queueFamily, 0); //Get first available queue
        commandPool = CreateCommandPool(device, queueFamily);
        renderPass = CreateRenderPass();
        imageAcquisitionFence = Queue::Fence::Create(device);
        mainCommandBuffer = CreateCommandBuffer(device, commandPool, Command::Pool::AllocateInfo::Level::Primary);
        drawCommandBuffer = CreateCommandBuffer(device, commandPool, Command::Pool::AllocateInfo::Level::Secondary);
    }
    void Loop()
    {
        FPSCounter fpsCounter(1);
        auto lastTime = std::chrono::high_resolution_clock::now();
        auto printTime = lastTime;
        auto uniformUpdateTime = lastTime;
        window.SetVSync(VSync);
        while (true) {
            
            fpsCounter.StartFrame();
            window.PushEvents();
            if (window.IsClosing()) break;
            if (!render) continue;
            const auto now = std::chrono::high_resolution_clock::now();
            const auto delta = std::chrono::duration<double, std::milli>(now - lastTime).count();
            lastTime = now;
            swapChainImage = window.AcquireNextImage(std::chrono::nanoseconds(15000000), nullptr, imageAcquisitionFence);
            Queue::Fence::WaitFor(device, imageAcquisitionFence, std::chrono::nanoseconds(15000000));
            Queue::Fence::Reset(device, { imageAcquisitionFence });
            projectionMatrix.Update();
            RecordMainCommandBuffer(delta);
            SubmitCommandBuffer(queue, mainCommandBuffer);
            window.Present(queue);
            fpsCounter.EndFrame();
            auto uniformUpdateDelta = std::chrono::duration<double, std::milli>(now - uniformUpdateTime).count();
            if (uniformUpdateDelta >= 16)
            {
                uniformUpdateTime = now;
                const float rotationAngle = uniformUpdateDelta * 0.0005;
                projectionMatrix.Set({ Mat4x4::Rotate(projectionMatrix.Get(), rotationAngle, Vec3(0.f, 1.f, 0.f)) });
            }
            if (std::chrono::duration<double, std::milli>(now - printTime).count() >= 48) {
                printTime = now;
                fpsCounter.Print();
            }
        }
    }
    RenderPass::Handle CreateRenderPass()
    {
		RenderPass::SubPassDescription subPassDescription{};
		RenderPass::AttachmentReference attachmentRef{};
		attachmentRef.location = 0;
		subPassDescription.colorAttachments = { attachmentRef };
        RenderPass::Info renderPassInfo{};
        RenderPass::AttachmentDescription colorAttachment;
        colorAttachment.loadOp = RenderPass::LoadOperation::Clear; //clear the attachment 0 on begin
        colorAttachment.storeOp = RenderPass::StoreOperation::Store; //write the result to attachment 0
        renderPassInfo.colorAttachments = { colorAttachment };
		renderPassInfo.subPasses = { subPassDescription };
        return RenderPass::Create(device, renderPassInfo);
    }
    void CreateShaderStages()
    {
        const auto compiler = ShaderCompiler::Create();
        {
            ShaderCompiler::Shader::Info shaderInfo;
            shaderInfo.type = ShaderCompiler::Shader::Type::Vertex;
            shaderInfo.entryPoint = "main";
            shaderInfo.source = {
                "#version 450                                               \n"
                "layout(location = 0) in vec2 inPosition;                   \n"
                "layout(location = 1) in vec3 inColor;                      \n"
                "layout(push_constant) uniform PushConstants {              \n"
                "   vec3 color;                                             \n"
                "} constants;                                               \n"
                "layout(binding = 0) uniform Projection {                   \n"
                "   mat4 matrix;                                            \n"
                "} proj;                                                    \n"
                "layout(location = 0) out vec3 fragColor;                   \n"
                "void main() {                                              \n"
                "   gl_Position = proj.matrix * vec4(inPosition, 0.0, 1.0); \n"
                "   fragColor = constants.color;                            \n"
                "}                                                          \n"
            };
            const auto vertexShader = ShaderCompiler::Shader::Create(compiler, shaderInfo);
            Shader::Module::Info shaderModuleInfo;
            shaderModuleInfo.code = ShaderCompiler::Shader::Compile(vertexShader);
            const auto shaderModule = Shader::Module::Create(device, shaderModuleInfo);
            Shader::Stage::Info shaderStageInfo;
            shaderStageInfo.entryPoint = "main";
            shaderStageInfo.stage = Shader::Stage::StageFlagBits::Vertex;
            shaderStageInfo.module = shaderModule;
            shaderStages.push_back(Shader::Stage::Create(device, shaderStageInfo));
        }
        {
            ShaderCompiler::Shader::Info shaderInfo;
            shaderInfo.type = ShaderCompiler::Shader::Type::Fragment;
            shaderInfo.entryPoint = "main";
            shaderInfo.source = {
                "#version 450                            \n"
                "layout(location = 0) out vec4 outColor; \n"
                "layout(location = 0) in vec3 fragColor; \n"
                "void main() {                           \n"
                "    outColor = vec4(fragColor, 1.0);    \n"
                "}                                       \n"
            };
            const auto fragmentShader = ShaderCompiler::Shader::Create(compiler, shaderInfo);
            Shader::Module::Info shaderModuleInfo;
            shaderModuleInfo.code = ShaderCompiler::Shader::Compile(fragmentShader);
            const auto shaderModule = Shader::Module::Create(device, shaderModuleInfo);
            Shader::Stage::Info shaderStageInfo;
            shaderStageInfo.entryPoint = "main";
            shaderStageInfo.stage = Shader::Stage::StageFlagBits::Fragment;
            shaderStageInfo.module = shaderModule;
            shaderStages.push_back(Shader::Stage::Create(device, shaderStageInfo));
        }
        
    }
    void CreateGraphicsPipeline()
    {
        ViewPort viewport;
        viewport.rect.offset = { 0, 0 };
        viewport.rect.extent = window.GetExtent();
        viewport.depthRange = { 0, 1 };
        Rect2D  scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = window.GetExtent();
        Pipeline::Layout::Handle layout;
        {
            Pipeline::Layout::Info layoutInfo;
            Pipeline::Layout::PushConstantRange pushConstantRange;
            pushConstantRange.size = sizeof(PushConstants);
            pushConstantRange.stage = Shader::Stage::StageFlagBits::Vertex;
            layoutInfo.setLayouts.push_back(projectionMatrix.GetDescriptorSetLayout());
            layoutInfo.pushConstants.push_back(pushConstantRange);
            layout = Pipeline::Layout::Create(device, layoutInfo);
        }
        graphicsPipelineInfo.layout = layout;
        graphicsPipelineInfo.renderPass = renderPass;
        graphicsPipelineInfo.rasterizationState.cullMode = Pipeline::RasterizationState::CullMode::None;
        graphicsPipelineInfo.inputAssemblyState.topology = Primitive::Topology::TriangleList;
        graphicsPipelineInfo.inputAssemblyState.primitiveRestartEnable = false;
        graphicsPipelineInfo.viewPortState.viewPorts = { viewport };
        graphicsPipelineInfo.viewPortState.scissors = { scissor };
        graphicsPipelineInfo.vertexInputState.attributeDescriptions = vertexBuffer.GetAttribsDescriptions();
        graphicsPipelineInfo.vertexInputState.bindingDescriptions   = vertexBuffer.GetBindingDescriptions();
        graphicsPipelineInfo.shaderPipelineState.stages = shaderStages;
		graphicsPipelineInfo.renderPass = renderPass;
		graphicsPipelineInfo.subPass = 0;
        //Everything else is left by default for now
        graphicsPipeline = Pipeline::Graphics::Create(device, graphicsPipelineInfo);
    }
    void CreateFrameBuffer()
    {
        {
            Image::Info imageInfo;
            imageInfo.type = Image::Type::Image2D;
            imageInfo.extent.width = window.GetExtent().width;
            imageInfo.extent.height = window.GetExtent().height;
            imageInfo.format = Image::Format::Uint8_Normalized_RGBA;
            imageInfo.mipLevels = 1;
            frameBufferImage = Image::Create(device, imageInfo);
        }
        Image::View::Handle imageView{};
        {
            Image::View::Info imageViewInfo;
            imageViewInfo.image = frameBufferImage;
            imageViewInfo.format = Image::Format::Uint8_Normalized_RGBA;
            imageViewInfo.type = Image::View::Type::View2D;
            imageViewInfo.subRange.layerCount = 1;
            imageView = Image::View::Create(device, imageViewInfo);
        }
        {
            FrameBuffer::Info frameBufferInfo{};
            frameBufferInfo.attachments.push_back(imageView);
            frameBufferInfo.extent.depth = 1;
            frameBufferInfo.extent.width = window.GetExtent().width;
            frameBufferInfo.extent.height = window.GetExtent().height;
            frameBufferInfo.renderPass = renderPass;
            frameBuffer = FrameBuffer::Create(device, frameBufferInfo);
        }
    }
    void RecordDrawCommandBuffer(const double a_Delta)
    {
        Command::Buffer::BeginInfo bufferBeginInfo{};
        bufferBeginInfo.flags = Command::Buffer::UsageFlagBits::None;
        Command::Buffer::Reset(drawCommandBuffer);
        Command::RenderPassBeginInfo renderPassBeginInfo{};
        renderPassBeginInfo.renderPass = renderPass;
        renderPassBeginInfo.framebuffer = frameBuffer;
        renderPassBeginInfo.renderArea.offset = { 0, 0 };
        renderPassBeginInfo.renderArea.extent = window.GetExtent();
        renderPassBeginInfo.colorClearValues.push_back(ColorValue(0.9529f, 0.6235f, 0.0941f, 1.f));
        Command::Buffer::Begin(drawCommandBuffer, bufferBeginInfo);
        UpdatePushConstants(a_Delta);
        Command::BeginRenderPass(drawCommandBuffer, renderPassBeginInfo, Command::SubPassContents::Inline);
        {
            Command::BindPipeline(drawCommandBuffer, Pipeline::BindingPoint::Graphics, graphicsPipeline);
            Command::BindDescriptorSets(drawCommandBuffer, Pipeline::BindingPoint::Graphics, graphicsPipelineInfo.layout, 0, projectionMatrix.GetDescriptorSets(), {});
            Command::BindVertexBuffers(drawCommandBuffer, 0, { vertexBuffer.GetBuffer() }, {0});
            Command::Draw(drawCommandBuffer, vertexBuffer.GetVertexNbr(), 1, 0, 0);
        }
        Command::EndRenderPass(drawCommandBuffer);
        Command::Buffer::End(drawCommandBuffer);
    }
    void UpdatePushConstants(const double a_Delta)
    {
        hue += 0.05 * a_Delta;
        hue = hue > 360 ? 0 : hue;
        PushConstants pushConstants;
        pushConstants.color = HSVtoRGB(hue, 0.5f, 1.f);
        std::vector<char> data(sizeof(pushConstants));
        std::memcpy(data.data(), &pushConstants, sizeof(pushConstants));
        Command::PushConstants(drawCommandBuffer, graphicsPipelineInfo.layout, 0, data);
    }
    void RecordMainCommandBuffer(float a_Delta)
    {
        drawCommandDelta += a_Delta;
        if (drawCommandDelta >= 0.032) {
            RecordDrawCommandBuffer(drawCommandDelta);
            drawCommandDelta = 0;
        }
        Command::Buffer::BeginInfo bufferBeginInfo{};
        bufferBeginInfo.flags = Command::Buffer::UsageFlagBits::None;
        Command::Buffer::Reset(mainCommandBuffer);
        Command::Buffer::Begin(mainCommandBuffer, bufferBeginInfo);
        {
            Command::ExecuteCommandBuffer(mainCommandBuffer, drawCommandBuffer);
            {
                Image::Copy imageCopy;
                imageCopy.extent.width  = window.GetExtent().width;
                imageCopy.extent.height = window.GetExtent().height;
                imageCopy.extent.depth  = 1;
                Command::CopyImage(
                    mainCommandBuffer,
                    frameBufferImage,
                    swapChainImage,
                    { imageCopy });
            }
        }
        Command::Buffer::End(mainCommandBuffer);
    }
    double                   drawCommandDelta{ 0 };
    float                    hue{ 0 };
    bool                     render{ false };
    PhysicalDevice::Handle   physicalDevice;
    Device::Handle           device;
    Window                   window;
    Image::Handle            swapChainImage;
    
    FrameBuffer::Handle      frameBuffer;
    Image::Handle            frameBufferImage;

    RenderPass::Handle       renderPass;
    Pipeline::Graphics::Info graphicsPipelineInfo;
    Pipeline::Handle         graphicsPipeline;
    Command::Pool::Handle    commandPool;
    Command::Buffer::Handle  mainCommandBuffer;
    Command::Buffer::Handle  drawCommandBuffer;
    Queue::Handle            queue;
    Queue::Fence::Handle     imageAcquisitionFence;

    VertexBuffer             vertexBuffer{ physicalDevice, device, std::vector<Vertex>{
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    }};
    std::vector<Shader::Stage::Handle>   shaderStages;
    UniformBuffer<Mat4x4> projectionMatrix{0, physicalDevice, device};
};

int main()
{
    int ret = 0;
    //basic setup as usual
    GraphicsPipelineTestApp testApp;
    testApp.window.Show();
    testApp.Loop();
    return ret;
}

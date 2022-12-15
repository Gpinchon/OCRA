#include <Common.hpp>
#include <Mat4x4.hpp>
#include <Mesh.hpp>
#include <PBRMaterial.hpp>
#include <Window.hpp>

#include <OCRA/Instance.hpp>
#include <OCRA/Device.hpp>
#include <OCRA/FrameBuffer.hpp>
#include <OCRA/RenderPass.hpp>
#include <OCRA/Surface.hpp>
#include <OCRA/SwapChain.hpp>
#include <OCRA/Command/Pool.hpp>
#include <OCRA/Command/Buffer.hpp>
#include <OCRA/Command/Draw.hpp>
#include <OCRA/Command/Scissor.hpp>
#include <OCRA/Command/ViewPort.hpp>
#include <OCRA/Descriptor/Set.hpp>
#include <OCRA/Pipeline/Layout.hpp>
#include <OCRA/Queue/Fence.hpp>
#include <OCRA/Common/Vec3.hpp>
#include <OCRA/Common/Vec_Math.hpp>

#include <OCRA/ShaderCompiler/Compiler.hpp>
#include <OCRA/ShaderCompiler/Shader.hpp>

#include <Windows.h>

#include <OCRA/Pipeline/Graphics.hpp>

using namespace OCRA;
constexpr auto VSync = false;
constexpr auto SwapChainImageNbr = 3;

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
            render = a_Width > 0 && a_Height > 0;
            if (render && !window.IsClosing()) {
                CreateFrameBuffer();
                CreateGraphicsPipeline();
            }
        };
        window.OnMaximize = window.OnResize;
        window.OnRestore = window.OnResize;
        window.OnMinimize = [this](const Window&, const uint32_t, const uint32_t) { render = false; };
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
            RecordMainCommandBuffer(delta);
            SubmitCommandBuffer(queue, mainCommandBuffer);
            window.Present(queue);
            fpsCounter.EndFrame();
            auto uniformUpdateDelta = std::chrono::duration<double, std::milli>(now - uniformUpdateTime).count();
            if (uniformUpdateDelta >= 16)
            {
                uniformUpdateTime = now;
                const float rotationAngle = uniformUpdateDelta * 0.0005;
                mesh.SetProjectionMatrix(Mat4x4::Rotate(mesh.GetProjectionMatrix(), rotationAngle, Vec3(0.f, 1.f, 0.f)));
                
            }
            if (std::chrono::duration<double, std::milli>(now - printTime).count() >= 48) {
                printTime = now;
                fpsCounter.Print();
            }
            firstLoop = false;
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
    void CreateGraphicsPipeline()
    {
        ViewPort viewport;
        viewport.rect.offset = { 0, 0 };
        viewport.rect.extent = window.GetExtent();
        viewport.depthRange = { 0, 1 };
        Rect2D  scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = window.GetExtent();
        Pipeline::Graphics::Info graphicsPipelineInfo;
        graphicsPipelineInfo.viewPortState.viewPorts = { viewport };
        graphicsPipelineInfo.viewPortState.scissors = { scissor };

        graphicsPipelineInfo.renderPass = renderPass;
        graphicsPipelineInfo.rasterizationState.cullMode = Pipeline::RasterizationState::CullMode::None;

        graphicsPipelineInfo.layout = mesh.GetPipelineLayout();
        graphicsPipelineInfo.inputAssemblyState = mesh.GetInputAssembly();
        graphicsPipelineInfo.vertexInputState.attributeDescriptions = mesh.GetVertexBuffer().GetAttribsDescriptions();
        graphicsPipelineInfo.vertexInputState.bindingDescriptions   = mesh.GetVertexBuffer().GetBindingDescriptions();
        graphicsPipelineInfo.shaderPipelineState.stages = mesh.GetShaderStages();

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
        mesh.Update();
        UpdateMeshColor(a_Delta);
        Command::Buffer::Begin(drawCommandBuffer, bufferBeginInfo);
        Command::BeginRenderPass(drawCommandBuffer, renderPassBeginInfo, Command::SubPassContents::Inline);
        {
            Command::BindPipeline(drawCommandBuffer, Pipeline::BindingPoint::Graphics, graphicsPipeline);
            mesh.Draw(drawCommandBuffer);
        }
        Command::EndRenderPass(drawCommandBuffer);
        Command::Buffer::End(drawCommandBuffer);
    }
    void UpdateMeshColor(const double a_Delta)
    {
        hue += 0.05 * a_Delta;
        hue = hue > 360 ? 0 : hue;
        PBRParameters param = mesh.GetMaterial().GetParameters<PBRParameters>();
        param.albedo = { HSVtoRGB(hue, 0.5f, 1.f), 1.f };
        mesh.GetMaterial().SetParameters(param);
    }
    void RecordMainCommandBuffer(float a_Delta)
    {
        drawCommandDelta += a_Delta;
        if (drawCommandDelta >= 32 || firstLoop) {
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
    bool                     firstLoop{ true };
    PhysicalDevice::Handle   physicalDevice;
    Device::Handle           device;
    Window                   window;
    Image::Handle            swapChainImage;
    
    FrameBuffer::Handle      frameBuffer;
    Image::Handle            frameBufferImage;

    Descriptor::Pool::Handle descriptorPool{ CreateDescriptorPool(device, 4096) };
    Mesh    mesh{ physicalDevice, device, descriptorPool,
        VertexBuffer(physicalDevice, device, std::vector<Vertex>{
            { {0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
        }),
        PBRMaterial(physicalDevice, device, descriptorPool) };

    RenderPass::Handle       renderPass;
    Pipeline::Handle         graphicsPipeline;
    Command::Pool::Handle    commandPool;
    Command::Buffer::Handle  mainCommandBuffer;
    Command::Buffer::Handle  drawCommandBuffer;
    Queue::Fence::Handle     imageAcquisitionFence;
    Queue::Handle            queue;
};

int main()
{
    int ret = 0;
    //basic setup as usual
    GraphicsPipelineTestApp testApp;
    testApp.window.Show();
    testApp.window.SetSwapChainImageNbr(SwapChainImageNbr);
    testApp.Loop();
    return ret;
}

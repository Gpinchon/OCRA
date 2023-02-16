#include <Common.hpp>
#include <Mat4x4.hpp>
#include <Mesh.hpp>
#include <PBRMaterial.hpp>
#include <Window.hpp>

#include <OCRA/OCRA.hpp>

#include <Windows.h>

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

struct GraphicsPipelineTestApp : TestApp
{
    GraphicsPipelineTestApp()
        : TestApp("Test_GraphicsPipeline")
        , window(Window(instance, physicalDevice, device, name, 1280, 720))
    {
        window.OnResize = [this](const Window&, const uint32_t a_Width, const uint32_t a_Height) {
            render = a_Width > 0 && a_Height > 0;
            if (render && !window.IsClosing()) {
                CreateFrameBuffer();
                CreateGraphicsPipeline();
                RecordDrawCommandBuffer();
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
        Queue::Semaphore::Info semaphoreInfo;
        semaphoreInfo.type = Queue::Semaphore::Type::Binary;
        drawSemaphore = Queue::Semaphore::Create(device, semaphoreInfo);
    }
    void Loop()
    {
        FPSCounter fpsCounter;
        auto lastTime = std::chrono::high_resolution_clock::now();
        auto printTime = lastTime;
        auto uniformUpdateTime = lastTime;
        window.SetVSync(VSync);
        fpsCounter.StartFrame();
        while (true) {
            window.PushEvents();
            if (window.IsClosing()) break;

            swapChainImage = window.AcquireNextImage({}, nullptr, imageAcquisitionFence);
            render = Queue::Fence::WaitFor(device, imageAcquisitionFence, std::chrono::nanoseconds(15000000));
            Queue::Fence::Reset(device, { imageAcquisitionFence });

            if (!render) continue;
            
            const auto now = std::chrono::high_resolution_clock::now();
            const auto delta = std::chrono::duration<double, std::milli>(now - lastTime).count();
            lastTime = now;

            auto uniformUpdateDelta = std::chrono::duration<double, std::milli>(now - uniformUpdateTime).count();
            if (uniformUpdateDelta >= 15)
            {
                uniformUpdateTime = now;
                const float rotationAngle = uniformUpdateDelta * 0.0005;
                mesh.SetProjectionMatrix(Mat4x4::Rotate(mesh.GetProjectionMatrix(), rotationAngle, Vec3(0.f, 1.f, 0.f)));
                UpdateMeshColor(uniformUpdateDelta);
                mesh.Update();
                if (firstLoop) RecordDrawCommandBuffer();
            }
            
            
            RecordMainCommandBuffer();
            SubmitCommandBuffer(queue, mainCommandBuffer, nullptr, drawSemaphore);
            window.Present(queue, drawSemaphore);
            fpsCounter.EndFrame();
            fpsCounter.StartFrame();
            
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
    void RecordDrawCommandBuffer()
    {
        Command::Buffer::BeginInfo bufferBeginInfo{};
        bufferBeginInfo.flags = Command::Buffer::UsageFlagBits::None;
        bufferBeginInfo.inheritanceInfo.emplace();
        Command::Buffer::Reset(drawCommandBuffer);
        Command::RenderPassBeginInfo renderPassBeginInfo{};
        renderPassBeginInfo.renderPass = renderPass;
        renderPassBeginInfo.framebuffer = frameBuffer;
        renderPassBeginInfo.renderArea.offset = { 0, 0 };
        renderPassBeginInfo.renderArea.extent = window.GetExtent();
        renderPassBeginInfo.colorClearValues.push_back(ColorValue(0.9529f, 0.6235f, 0.0941f, 1.f));
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
    void RecordMainCommandBuffer()
    {
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
    float                    hue{ 0 };
    bool                     render{ false };
    bool                     firstLoop{ true };
    Window                   window;
    Image::Handle            swapChainImage;
    
    FrameBuffer::Handle      frameBuffer;
    Image::Handle            frameBufferImage;

    Mesh    mesh{ physicalDevice, device,
        VertexBuffer(physicalDevice, device, std::vector<DefaultVertex>{
            { {  0.0f, -0.5f, 0.f }, { 1.0f, 0.0f, 0.0f }, { 0.f, 1.f }},
            { {  0.5f,  0.5f, 0.f }, { 0.0f, 1.0f, 0.0f }, { 1.f, 0.f }},
            { { -0.5f,  0.5f, 0.f }, { 0.0f, 0.0f, 1.0f }, { 1.f, 1.f }}
        }),
        PBRMaterial(physicalDevice, device) };

    RenderPass::Handle       renderPass;
    Pipeline::Handle         graphicsPipeline;
    Command::Pool::Handle    commandPool;
    Command::Buffer::Handle  mainCommandBuffer;
    Command::Buffer::Handle  drawCommandBuffer;
    Queue::Semaphore::Handle drawSemaphore; //To be signaled when drawing is done
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

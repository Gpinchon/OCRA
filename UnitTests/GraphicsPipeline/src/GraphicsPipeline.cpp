#include <Common.hpp>
#include <Mat4x4.hpp>
#include <Mesh.hpp>
#include <PBRMaterial.hpp>
#include <Window.hpp>

#include <OCRA/OCRA.hpp>

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
        const auto queueFamily = PhysicalDevice::FindQueueFamily(physicalDevice, QueueFlagBits::Graphics);
        queue = Device::GetQueue(device, queueFamily, 0); //Get first available queue
        commandPool = CreateCommandPool(device, queueFamily);
        //renderPass = CreateRenderPass();
        imageAcquisitionFence = CreateFence(device);
        mainCommandBuffer = CreateCommandBuffer(commandPool, CommandBufferLevel::Primary);
        drawCommandBuffer = CreateCommandBuffer(commandPool, CommandBufferLevel::Secondary);
        CreateSemaphoreInfo semaphoreInfo;
        semaphoreInfo.type = SemaphoreType::Binary;
        drawSemaphore = CreateSemaphore(device, semaphoreInfo);
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
            render = Fence::WaitFor(imageAcquisitionFence, IgnoreTimeout);
            Fence::Reset({ imageAcquisitionFence });

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
            QueueSubmitInfo submitInfo;
            submitInfo.commandBuffers = { mainCommandBuffer };
            //submitInfo.waitSemaphores = { { imageAcquisitionSemaphore, 0, PipelineStageFlagBits::BottomOfPipe } };
            submitInfo.signalSemaphores = { { drawSemaphore } };
            Queue::Submit(queue, { submitInfo }/*, completeBufferFence*/);
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
    //RenderPass::Handle CreateRenderPass()
    //{
    //    SubPassDescription subPassDescription{};
    //    AttachmentReference attachmentRef{};
    //    attachmentRef.location = 0;
    //    subPassDescription.colorAttachments = { attachmentRef };
    //    CreateRenderPassInfo renderPassInfo{};
    //    AttachmentDescription colorAttachment;
    //    colorAttachment.loadOp  = LoadOp::Clear; //clear the attachment 0 on begin
    //    colorAttachment.storeOp = StoreOp::Store; //write the result to attachment 0
    //    renderPassInfo.colorAttachments = { colorAttachment };
    //    renderPassInfo.subPasses = { subPassDescription };
    //    return Device::CreateRenderPass(device, renderPassInfo);
    //}
    void CreateGraphicsPipeline()
    {
        ViewPort viewport;
        viewport.rect.offset = { 0, 0 };
        viewport.rect.extent = window.GetExtent();
        viewport.depthRange = { 0, 1 };
        Rect2D  scissor{};
        scissor.offset = { 0, 0 };
        scissor.extent = window.GetExtent();
        CreatePipelineGraphicsInfo graphicsPipelineInfo;
        graphicsPipelineInfo.viewPortState.viewPorts = { viewport };
        graphicsPipelineInfo.viewPortState.scissors = { scissor };

        graphicsPipelineInfo.dynamicState.dynamicStates = { DynamicState::VertexInput };

        //graphicsPipelineInfo.renderPass = renderPass;
        graphicsPipelineInfo.rasterizationState.cullMode = CullMode::None;

        graphicsPipelineInfo.layout = mesh.GetPipelineLayout();
        graphicsPipelineInfo.inputAssemblyState = mesh.GetInputAssembly();
        graphicsPipelineInfo.shaderPipelineState.stages = mesh.GetShaderStages();

        graphicsPipelineInfo.subPass = 0;
        //Everything else is left by default for now
        graphicsPipeline = CreatePipelineGraphics(device, graphicsPipelineInfo);
    }
    void CreateFrameBuffer()
    {
        {
            CreateImageInfo imageInfo;
            imageInfo.type = ImageType::Image2D;
            imageInfo.extent.width = window.GetExtent().width;
            imageInfo.extent.height = window.GetExtent().height;
            imageInfo.format = Format::Uint8_Normalized_RGBA;
            imageInfo.mipLevels = 1;
            frameBufferImage = CreateImage(device, imageInfo);
        }
        {
            CreateImageViewInfo imageViewInfo;
            imageViewInfo.image = frameBufferImage;
            imageViewInfo.format = Format::Uint8_Normalized_RGBA;
            imageViewInfo.type = ImageViewType::View2D;
            imageViewInfo.subRange.layerCount = 1;
            frameBufferImageView = CreateImageView(device, imageViewInfo);
        }
        {
            //CreateFrameBufferInfo frameBufferInfo{};
            //frameBufferInfo.attachments.push_back(frameBufferImageView);
            //frameBufferInfo.extent.depth = 1;
            //frameBufferInfo.extent.width = window.GetExtent().width;
            //frameBufferInfo.extent.height = window.GetExtent().height;
            //frameBufferInfo.renderPass = renderPass;
            //frameBuffer = Device::CreateFrameBuffer(device, frameBufferInfo);
        }
    }
    void RecordDrawCommandBuffer()
    {
        CommandBufferBeginInfo bufferBeginInfo{};
        bufferBeginInfo.flags = CommandBufferUsageFlagBits::None;
        //bufferBeginInfo.inheritanceInfo.emplace();
        Command::Buffer::Reset(drawCommandBuffer);
        //RenderPassBeginInfo renderPassBeginInfo{};
        //renderPassBeginInfo.renderPass = renderPass;
        //renderPassBeginInfo.framebuffer = frameBuffer;
        //renderPassBeginInfo.renderArea.offset = { 0, 0 };
        //renderPassBeginInfo.renderArea.extent = window.GetExtent();
        //renderPassBeginInfo.colorClearValues.push_back(ColorValue(0.9529f, 0.6235f, 0.0941f, 1.f));
        RenderingAttachmentInfo renderingAttachment{};
        renderingAttachment.clearValue  = ColorValue(0.9529f, 0.6235f, 0.0941f, 1.f);
        renderingAttachment.imageView   = frameBufferImageView;
        renderingAttachment.imageLayout = ImageLayout::General;
        renderingAttachment.loadOp      = LoadOp::Clear;
        renderingAttachment.storeOp     = StoreOp::Store;
        RenderingInfo renderingInfo{};
        renderingInfo.area.offset = { 0, 0 };
        renderingInfo.area.extent = window.GetExtent();
        renderingInfo.colorAttachments.push_back(renderingAttachment);
        renderingInfo.layerCount = 1;

        Command::Buffer::Begin(drawCommandBuffer, bufferBeginInfo);
        //Command::BeginRenderPass(drawCommandBuffer, renderPassBeginInfo, SubPassContents::Inline);
        Command::BeginRendering(drawCommandBuffer, renderingInfo);
        {
            Command::BindPipeline(drawCommandBuffer, graphicsPipeline);
            mesh.Draw(drawCommandBuffer);
        }
        Command::EndRendering(drawCommandBuffer);
        //Command::EndRenderPass(drawCommandBuffer);
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
        CommandBufferBeginInfo bufferBeginInfo{};
        bufferBeginInfo.flags = CommandBufferUsageFlagBits::None;
        Command::Buffer::Reset(mainCommandBuffer);
        Command::Buffer::Begin(mainCommandBuffer, bufferBeginInfo);
        {
            Command::ExecuteCommandBuffer(mainCommandBuffer, drawCommandBuffer);
            {
                ImageCopy imageCopy;
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
    
    //FrameBuffer::Handle      frameBuffer;
    Image::Handle            frameBufferImage;
    Image::View::Handle      frameBufferImageView;

    Mesh    mesh{ device,
        VertexBuffer(device, std::vector<DefaultVertex>{
            { {  0.0f, -0.5f, 0.f }, { 1.0f, 0.0f, 0.0f }, { 0.f, 1.f }},
            { {  0.5f,  0.5f, 0.f }, { 0.0f, 1.0f, 0.0f }, { 1.f, 0.f }},
            { { -0.5f,  0.5f, 0.f }, { 0.0f, 0.0f, 1.0f }, { 1.f, 1.f }}
        }),
        PBRMaterial(physicalDevice, device) };

    //RenderPass::Handle       renderPass;
    Pipeline::Handle         graphicsPipeline;
    Command::Pool::Handle    commandPool;
    Command::Buffer::Handle  mainCommandBuffer;
    Command::Buffer::Handle  drawCommandBuffer;
    Semaphore::Handle drawSemaphore; //To be signaled when drawing is done
    Fence::Handle     imageAcquisitionFence;
    Queue::Handle            queue;
};

int main()
{
    int ret = 0;
    GraphicsPipelineTestApp testApp;
    testApp.window.Show();
    testApp.window.SetSwapChainImageNbr(SwapChainImageNbr);
    testApp.Loop();
    return ret;
}

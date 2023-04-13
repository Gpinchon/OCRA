#include <Common.hpp>
#include <Mat4x4.hpp>
#include <Mesh.hpp>
#include <PBRMaterial.hpp>
#include <Window.hpp>

#include <OCRA/OCRA.hpp>

using namespace OCRA;

constexpr auto VSync = false;
constexpr auto SwapChainImageNbr = 5;
constexpr auto Width  = 256;
constexpr auto Height = 256;
constexpr auto Samples = SampleCount::Count8;
constexpr auto Resolve = Samples == SampleCount::Count1 ? ResolveMode::None : ResolveMode::Average;

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
        , window(instance, physicalDevice, device, name, Width, Height)
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
        mainCommandBuffer = CreateCommandBuffer(commandPool, CommandBufferLevel::Primary);
        drawCommandBuffer = CreateCommandBuffer(commandPool, CommandBufferLevel::Secondary);
        CreateSemaphoreInfo semaphoreInfo;
        semaphoreInfo.type = SemaphoreType::Binary;
    }
    void CreateSyncObjects()
    {
        auto swapchainNbr = window.GetSwapChainImageNbr();
        CreateSemaphoreInfo semaphoreInfo;
        semaphoreInfo.type = SemaphoreType::Binary;
        completeBufferSemaphores.resize(swapchainNbr);
        completeBufferFences.resize(swapchainNbr);
        imageAcquisitionSemaphores.resize(swapchainNbr);
        imageAcquisitionFences.resize(swapchainNbr);
        for (auto& semaphore : completeBufferSemaphores) semaphore = CreateSemaphore(device, semaphoreInfo);
        for (auto& fence     : completeBufferFences)     fence = CreateFence(device, FenceStatus::Unsignaled);
        for (auto& semaphore : imageAcquisitionSemaphores) semaphore = CreateSemaphore(device, semaphoreInfo);
        for (auto& fence     : imageAcquisitionFences)     fence = CreateFence(device, FenceStatus::Unsignaled);
    }
    void Loop()
    {
        FPSCounter fpsCounter;
        auto lastTime = std::chrono::high_resolution_clock::now();
        auto printTime = lastTime;
        auto uniformUpdateTime = lastTime;
        window.SetVSync(VSync);
        CreateSyncObjects();
        fpsCounter.StartFrame();
        while (true) {
            window.PushEvents();
            if (window.IsClosing()) break;
            if (!render) continue;

            uint32_t imageIndex = window.GetNextImageIndex();
            auto& completeBufferSemaphore = completeBufferSemaphores.at(imageIndex);
            auto& completeBufferFence     = completeBufferFences.at(imageIndex);
            auto& imageAcquisitionSemaphore = imageAcquisitionSemaphores.at(imageIndex);
            auto& imageAcquisitionFence     = imageAcquisitionFences.at(imageIndex);

            const auto swapChainImage = window.AcquireNextImage(
                std::chrono::nanoseconds(0),
                imageAcquisitionSemaphore, imageAcquisitionFence);
            Fence::WaitFor(imageAcquisitionFence, IgnoreTimeout);

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

            RecordMainCommandBuffer(swapChainImage);

            QueueSubmitInfo submitInfo;
            submitInfo.commandBuffers = { mainCommandBuffer };
            submitInfo.waitSemaphores = { { imageAcquisitionSemaphore, 0, PipelineStageFlagBits::BottomOfPipe } };
            submitInfo.signalSemaphores = { { completeBufferSemaphore } };
            Queue::Submit(queue, { submitInfo }, completeBufferFence);

            window.Present(queue, completeBufferSemaphore);

            fpsCounter.EndFrame();
            fpsCounter.StartFrame();
            if (std::chrono::duration<double, std::milli>(now - printTime).count() >= 48) {
                printTime = now;
                fpsCounter.Print();
            }
            firstLoop = false;

            Fence::WaitFor(completeBufferFence, IgnoreTimeout);
            Fence::Reset({ completeBufferFence, imageAcquisitionFence });
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
        CreatePipelineGraphicsInfo graphicsPipelineInfo;
        graphicsPipelineInfo.viewPortState.viewPorts = { viewport };
        graphicsPipelineInfo.viewPortState.scissors = { scissor };

        graphicsPipelineInfo.colorBlendState.attachments = { PipelineColorBlendAttachmentState{} };

        graphicsPipelineInfo.dynamicState.dynamicStates = { DynamicState::VertexInput };

        graphicsPipelineInfo.rasterizationState.cullMode = CullMode::None;

        //graphicsPipelineInfo.layout = mesh.GetPipelineLayout();
        graphicsPipelineInfo.bindings = mesh.GetDescriptorSetLayoutBindings();
        graphicsPipelineInfo.inputAssemblyState = mesh.GetInputAssembly();
        graphicsPipelineInfo.shaderPipelineState.stages = mesh.GetShaderStages();
        //graphicsPipelineInfo.multisampleState.sampleShadingEnable = true;
        graphicsPipelineInfo.multisampleState.rasterizationSamples = Samples;

        graphicsPipelineInfo.renderingInfo.colorAttachmentFormats = { Format::Uint8_Normalized_RGBA };

        //Everything else is left by default for now
        graphicsPipeline = CreatePipelineGraphics(device, graphicsPipelineInfo);
    }
    void CreateFrameBuffer()
    {
        {
            CreateImageInfo imageInfo;
            imageInfo.type = ImageType::Image2D;
            imageInfo.usage = ImageUsageFlagBits::ColorAttachment | ImageUsageFlagBits::TransferSrc;
            imageInfo.samples = Samples;
            imageInfo.extent.width  = window.GetExtent().width;
            imageInfo.extent.height = window.GetExtent().height;
            imageInfo.extent.depth  = 1;
            imageInfo.format = Format::Uint8_Normalized_RGBA;
            imageInfo.arrayLayers = 1;
            imageInfo.mipLevels = 1;
            renderImage = CreateImage(device, imageInfo);
            CreateImageViewInfo imageViewInfo;
            imageViewInfo.image = renderImage;
            imageViewInfo.format = Format::Uint8_Normalized_RGBA;
            imageViewInfo.type = ImageViewType::View2D;
            imageViewInfo.subRange.layerCount = 1;
            imageViewInfo.subRange.aspects = ImageAspectFlagBits::Color;
            renderImageView = CreateImageView(device, imageViewInfo);
        }
        if (Samples != SampleCount::Count1)
        {
            CreateImageInfo imageInfo;
            imageInfo.type = ImageType::Image2D;
            imageInfo.usage = ImageUsageFlagBits::ColorAttachment | ImageUsageFlagBits::TransferSrc;
            imageInfo.extent.width  = window.GetExtent().width;
            imageInfo.extent.height = window.GetExtent().height;
            imageInfo.extent.depth  = 1;
            imageInfo.format = Format::Uint8_Normalized_RGBA;
            imageInfo.arrayLayers = 1;
            imageInfo.mipLevels = 1;
            resolveImage = CreateImage(device, imageInfo);
            CreateImageViewInfo imageViewInfo;
            imageViewInfo.image = resolveImage;
            imageViewInfo.format = Format::Uint8_Normalized_RGBA;
            imageViewInfo.type = ImageViewType::View2D;
            imageViewInfo.subRange.layerCount = 1;
            imageViewInfo.subRange.aspects = ImageAspectFlagBits::Color;
            resolveImageView = CreateImageView(device, imageViewInfo);
        }
    }
    void RecordDrawCommandBuffer()
    {
        CommandBufferBeginInfo bufferBeginInfo{};
        bufferBeginInfo.flags = CommandBufferUsageFlagBits::None;
        bufferBeginInfo.inheritanceInfo.emplace();
        Command::Buffer::Reset(drawCommandBuffer);

        RenderingAttachmentInfo renderingAttachment{};
        renderingAttachment.clearValue         = ColorValue(0.9529f, 0.6235f, 0.0941f, 1.f);
        renderingAttachment.imageView          = renderImageView;
        renderingAttachment.imageViewResolve   = Samples == SampleCount::Count1 ? nullptr : resolveImageView;
        renderingAttachment.imageLayout        = ImageLayout::General;
        renderingAttachment.imageLayoutResolve = ImageLayout::General;
        renderingAttachment.loadOp             = LoadOp::Clear;
        renderingAttachment.storeOp            = StoreOp::Store;
        RenderingInfo renderingInfo{};
        renderingInfo.area.offset = { 0, 0 };
        renderingInfo.area.extent = window.GetExtent();
        renderingInfo.colorAttachments.push_back(renderingAttachment);
        renderingInfo.layerCount = 1;
        renderingInfo.resolveMode = Resolve;

        const auto descriptorWrites = mesh.GetDescriptorWrites();

        Command::Buffer::Begin(drawCommandBuffer, bufferBeginInfo);
        if (!descriptorWrites.empty()) {
            Command::PushDescriptorSet(drawCommandBuffer, graphicsPipeline, descriptorWrites);
        }
        Command::BeginRendering(drawCommandBuffer, renderingInfo);
        {
            Command::BindPipeline(drawCommandBuffer, graphicsPipeline);
            mesh.Draw(drawCommandBuffer, graphicsPipeline);
        }
        Command::EndRendering(drawCommandBuffer);
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
    void RecordMainCommandBuffer(const Image::Handle& a_SwapChainImage)
    {
        CommandBufferBeginInfo bufferBeginInfo{};
        bufferBeginInfo.flags = CommandBufferUsageFlagBits::None;
        Command::Buffer::Reset(mainCommandBuffer);
        Command::Buffer::Begin(mainCommandBuffer, bufferBeginInfo);
        {
            Command::ExecuteCommandBuffer(mainCommandBuffer, drawCommandBuffer);
            {
                ImageBlit imageBlit;
                imageBlit.srcSubresource.aspects = ImageAspectFlagBits::Color;
                imageBlit.srcSubresource.layerCount = 1;
                imageBlit.srcOffsets[0] = Offset3D(0, 0, 0);
                imageBlit.srcOffsets[1] = Offset3D(window.GetExtent().width, window.GetExtent().height, 1);

                imageBlit.dstSubresource.aspects = ImageAspectFlagBits::Color;
                imageBlit.srcSubresource.layerCount = 1;
                imageBlit.dstOffsets[0] = Offset3D(0, 0, 0);
                imageBlit.dstOffsets[1] = Offset3D(window.GetExtent().width, window.GetExtent().height, 1);

                auto srcImage = Samples == SampleCount::Count1 ? renderImage : resolveImage;
                auto dstImage = a_SwapChainImage;

                //We do a blit here to perform necessary conversions
                Command::BlitImage(mainCommandBuffer,
                    srcImage,
                    dstImage,
                    { imageBlit }, Filter::Nearest);

                ImageSubresourceRange range{};
                range.aspects = ImageAspectFlagBits::Color;
                range.levelCount = 1;
                range.layerCount = 1;
                Command::TransitionImageLayout(
                    mainCommandBuffer, { a_SwapChainImage, range,
                    ImageLayout::TransferDstOptimal,
                    ImageLayout::PresentSrc });
            }
        }
        Command::Buffer::End(mainCommandBuffer);
    }
    float                    hue{ 0 };
    bool                     render{ false };
    bool                     firstLoop{ true };
    Window                   window;
    
    Image::Handle            renderImage;
    Image::Handle            resolveImage;
    Image::View::Handle      renderImageView;
    Image::View::Handle      resolveImageView;

    Mesh    mesh{ device,
        VertexBuffer(device, std::vector<DefaultVertex>{
            { {  0.0f, -0.5f, 0.f }, { 1.0f, 0.0f, 0.0f }, { 0.f, 1.f }},
            { {  0.5f,  0.5f, 0.f }, { 0.0f, 1.0f, 0.0f }, { 1.f, 0.f }},
            { { -0.5f,  0.5f, 0.f }, { 0.0f, 0.0f, 1.0f }, { 1.f, 1.f }}
        }),
        PBRMaterial(physicalDevice, device) };
    Pipeline::Handle         graphicsPipeline;
    Command::Pool::Handle    commandPool;
    Command::Buffer::Handle  mainCommandBuffer;
    Command::Buffer::Handle  drawCommandBuffer;
    Queue::Handle            queue;
    std::vector<Semaphore::Handle> completeBufferSemaphores;
    std::vector<Fence::Handle>     completeBufferFences;
    std::vector<Semaphore::Handle> imageAcquisitionSemaphores;
    std::vector<Fence::Handle>     imageAcquisitionFences;
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

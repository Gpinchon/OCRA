#include <Common.hpp>
#include <Window.hpp>

#include <OCRA/OCRA.hpp>

using namespace OCRA;

constexpr auto VSync = false;
constexpr auto SWAPCHAIN_IMAGE_NBR = 5;

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

struct SwapChainTestApp : TestApp
{
    SwapChainTestApp()
        : TestApp("Test_SwapChain")
        , window(Window(instance, physicalDevice, device, name, 1280, 720))
    {
        window.OnResize = [this](const Window&, const uint32_t a_Width, const uint32_t a_Height) {
            render = a_Width > 0 && a_Height > 0;
        };
        window.OnMaximize = window.OnResize;
        window.OnRestore = window.OnResize;
        window.OnMinimize = [this](const Window&, const uint32_t, const uint32_t) { render = false; };
        const auto queueFamily = PhysicalDevice::FindQueueFamily(physicalDevice, QueueFlagsBits::Graphics);
        queue = GetQueue(device, queueFamily, 0); //Get first available queue
        commandPool = CreateCommandPool(device, queueFamily);
        commandBuffer = CreateCommandBuffer(commandPool, CommandBufferLevel::Primary);
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
        auto printTime = std::chrono::high_resolution_clock::now();
        window.SetVSync(VSync);
        fpsCounter.StartFrame();
        CreateSyncObjects();
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
            
            RecordClearCommandBuffer(swapChainImage);

            QueueSubmitInfo submitInfo;
            submitInfo.commandBuffers   = { commandBuffer };
            submitInfo.waitSemaphores   = { { imageAcquisitionSemaphore, 0, PipelineStageFlagBits::BottomOfPipe } };
            submitInfo.signalSemaphores = { { completeBufferSemaphore } };
            Queue::Submit(queue, { submitInfo }, completeBufferFence);

            window.Present(queue, completeBufferSemaphore);

            Fence::WaitFor(completeBufferFence, IgnoreTimeout);
            Fence::Reset({ completeBufferFence, imageAcquisitionFence });
            fpsCounter.EndFrame();
            fpsCounter.StartFrame();
            const auto now = std::chrono::high_resolution_clock::now();
            if (std::chrono::duration<double, std::milli>(now - printTime).count() >= 48) {
                printTime = now;
                fpsCounter.Print();
            }
        }
    }
    void RecordClearCommandBuffer(const Image::Handle& a_Image)
    {
        static float hue = 0;
        static auto lastTime = std::chrono::high_resolution_clock::now();
        const auto now   = std::chrono::high_resolution_clock::now();
        const auto delta = std::chrono::duration<double, std::milli>(now - lastTime).count();
        lastTime = now;
        hue += 0.05 * delta;
        hue = hue > 360 ? 0 : hue;
        const auto color = HSVtoRGB(hue, 0.5f, 1.f);
        CommandBufferBeginInfo bufferBeginInfo;
        bufferBeginInfo.flags = CommandBufferUsageFlagBits::SimultaneousUse;
        Command::Buffer::Reset(commandBuffer);
        Command::Buffer::Begin(commandBuffer, bufferBeginInfo);
        {
            ImageSubresourceRange range{};
            range.aspect = ImageAspectFlagBits::Color;
            range.levelCount = 1;
            range.layerCount = 1;

            Command::TransitionImageLayout(
                commandBuffer, a_Image, range,
                ImageLayout::Undefined,
                ImageLayout::TransferDstOptimal);
            Command::ClearColorImage(commandBuffer, a_Image,
                ImageLayout::TransferDstOptimal,
                ColorValue{ color.r, color.g, color.b, 1.f },
                { range });
            Command::TransitionImageLayout(
                commandBuffer, a_Image, range,
                ImageLayout::TransferDstOptimal,
                ImageLayout::PresentSrc);
        }
        Command::Buffer::End(commandBuffer);
    }
    bool                     render{ false };
    Window                   window;
    Queue::Handle            queue;
    Command::Pool::Handle    commandPool;
    Command::Buffer::Handle  commandBuffer;
    std::vector<Semaphore::Handle> completeBufferSemaphores;
    std::vector<Fence::Handle>     completeBufferFences;
    std::vector<Semaphore::Handle> imageAcquisitionSemaphores;
    std::vector<Fence::Handle>     imageAcquisitionFences;
};

int main()
{
    int ret = 0;
    SwapChainTestApp testApp;
    testApp.window.Show();
    testApp.window.SetSwapChainImageNbr(SWAPCHAIN_IMAGE_NBR);
    testApp.Loop();
    return ret;
}

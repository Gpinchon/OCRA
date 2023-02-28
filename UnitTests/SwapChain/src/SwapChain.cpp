#include <Common.hpp>
#include <Window.hpp>

#include <OCRA/OCRA.hpp>

using namespace OCRA;

constexpr auto VSync = false;
constexpr auto SWAPCHAIN_IMAGE_NBR = 2;

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
        window.OnResize = [this](const Window&, const uint32_t a_Width, const uint32_t a_Height) { render = a_Width > 0 && a_Height > 0; };
        window.OnMaximize = window.OnResize;
        window.OnRestore = window.OnResize;
        window.OnMinimize = [this](const Window&, const uint32_t, const uint32_t) { render = false; };
        const auto queueFamily = PhysicalDevice::FindQueueFamily(physicalDevice, QueueFlagsBits::Graphics);
        queue = Device::GetQueue(device, queueFamily, 0); //Get first available queue
        imageAcquisitionFence = CreateFence(device);
        commandPool = CreateCommandPool(device, queueFamily);
        commandBuffer = CreateCommandBuffer(commandPool, CommandBufferLevel::Primary);
        CreateSemaphoreInfo semaphoreInfo;
        semaphoreInfo.type = SemaphoreType::Binary;
        drawWaitSemaphore = CreateSemaphore(device, semaphoreInfo);
        drawSignalSemaphore = Device::CreateSemaphore(device, semaphoreInfo);
    }
    void Loop()
    {
        FPSCounter fpsCounter;
        auto printTime = std::chrono::high_resolution_clock::now();
        window.SetVSync(VSync);
        fpsCounter.StartFrame();
        while (true) {
            window.PushEvents();
            if (window.IsClosing()) break;

            const auto swapChainImage = window.AcquireNextImage(std::chrono::nanoseconds(0), drawWaitSemaphore, imageAcquisitionFence);
            render = Fence::WaitFor(imageAcquisitionFence, Fence::IgnoreTimeout);
            Fence::Reset({ imageAcquisitionFence });

            if (!render) continue;
            
            RecordClearCommandBuffer(swapChainImage);
            SubmitCommandBuffer(queue, commandBuffer, drawWaitSemaphore, drawSignalSemaphore);
            window.Present(queue, drawSignalSemaphore);
            
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
        const auto now = std::chrono::high_resolution_clock::now();
        const auto delta = std::chrono::duration<double, std::milli>(now - lastTime).count();
        lastTime = now;
        hue += 0.05 * delta;
        hue = hue > 360 ? 0 : hue;
        const auto color = HSVtoRGB(hue, 0.5f, 1.f);
        CommandBufferBeginInfo bufferBeginInfo;
        bufferBeginInfo.flags = CommandBufferUsageFlagBits::None;
        Command::Buffer::Reset(commandBuffer);
        Command::Buffer::Begin(commandBuffer, bufferBeginInfo);
        {
            ColorValue clearColor{ color.r, color.g, color.b, 1.f };
            ImageSubresourceRange range{};
            range.levelCount = 1;
            Command::ClearColorImage(commandBuffer, a_Image, ImageLayout::Unknown, clearColor, { range });
        }
        Command::Buffer::End(commandBuffer);
    }
    bool                     render{ false };
    Window                   window;
    Queue::Handle            queue;
    Semaphore::Handle drawWaitSemaphore;
    Semaphore::Handle drawSignalSemaphore;
    Fence::Handle     imageAcquisitionFence;
    Command::Pool::Handle    commandPool;
    Command::Buffer::Handle  commandBuffer;
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

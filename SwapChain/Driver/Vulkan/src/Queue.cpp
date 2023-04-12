#include <VK/Assert.hpp>
#include <VK/Queue.hpp>
#include <VK/Semaphore.hpp>
#include <VK/SwapChain/SwapChain.hpp>

#include <OCRA/Core.hpp>
#include <OCRA/SwapChain.hpp>

namespace OCRA::Queue
{
void Present(
    const Queue::Handle& a_Queue,
    const SwapChainPresentInfo& a_PresentInfo)
{
    const auto swapChainCount = a_PresentInfo.swapChains.size();
    std::vector<vk::SwapchainKHR> swapChains(swapChainCount);
    std::vector<uint32_t> imageIndice(swapChainCount);
    for (auto i = 0u; i < swapChainCount; ++i) {
        const auto& swapChain = *a_PresentInfo.swapChains.at(i);
        swapChains.at(i)  = *swapChain;
        imageIndice.at(i) = swapChain.imageIndex;
    }
    const auto waitSemaphoreCount = a_PresentInfo.waitSemaphores.size();
    std::vector<vk::Semaphore> waitSemaphores(waitSemaphoreCount);
    for (auto i = 0u; i < waitSemaphoreCount; ++i) {
        waitSemaphores.at(i) = **a_PresentInfo.waitSemaphores.at(i);
    }
    vk::PresentInfoKHR info(waitSemaphores, swapChains, imageIndice);
    VK_INVOKE(a_Queue->presentKHR(info));
}
}
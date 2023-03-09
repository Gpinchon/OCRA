#include <VK/Buffer.hpp>
#include <VK/Device.hpp>
#include <VK/Enums.hpp>
#include <VK/Flags.hpp>
#include <VK/Image.hpp>
#include <VK/PhysicalDevice.hpp>
#include <VK/Structs.hpp>

#include <OCRA/OCRA.hpp>

namespace OCRA::Image
{
void CopyBufferToImage(
    const OCRA::Buffer::Handle& a_SrcBuffer,
    const Image::Handle& a_DstImage,
    const std::vector<ImageBufferCopy>& a_Regions)
{
    auto& srcBuffer = *a_SrcBuffer;
    auto& dstImage = *a_DstImage;
    auto& device = srcBuffer.device;
    std::vector<vk::BufferImageCopy> vkCopies(a_Regions.size());
    for (auto i = 0u; i < vkCopies.size(); ++i) {
        auto& ocCopy = a_Regions.at(i);
        auto& vkCopy = vkCopies.at(i);
        vkCopy.bufferImageHeight = ocCopy.bufferImageHeight;
        vkCopy.bufferOffset = ocCopy.bufferOffset;
        vkCopy.bufferRowLength = ocCopy.bufferRowLength;
        vkCopy.imageExtent = GetVkExtent3D(ocCopy.imageExtent);
        vkCopy.imageOffset = GetVkOffset3D(ocCopy.imageOffset);
        vkCopy.imageSubresource = GetVkImageSubresourceLayers(ocCopy.imageSubresource);
    }
    vk::raii::CommandBuffer transferCommandBuffer = nullptr;
    {
        vk::CommandBufferAllocateInfo commandBufferInfo;
        commandBufferInfo.commandBufferCount = 1;
        commandBufferInfo.commandPool = *device.transferCommandPool;
        commandBufferInfo.level = vk::CommandBufferLevel::ePrimary;
        transferCommandBuffer = std::move(device.allocateCommandBuffers(commandBufferInfo).front());
    }
    {
        vk::CommandBufferBeginInfo cmdBufferBeginInfo;
        cmdBufferBeginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
        transferCommandBuffer.begin(cmdBufferBeginInfo);
        transferCommandBuffer.copyBufferToImage(
            *srcBuffer, *dstImage,
            vk::ImageLayout::eTransferDstOptimal,
            vkCopies);
        transferCommandBuffer.end();
    }
    {
        vk::SubmitInfo submitInfo;
        device.transferQueue.submit({ submitInfo }, nullptr);
    }
    device.transferQueue.waitIdle();
}

void CopyImageToBuffer(
    const OCRA::Buffer::Handle& a_DstBuffer,
    const Image::Handle& a_SrcImage,
    const std::vector<ImageBufferCopy>& a_Regions)
{

}
}
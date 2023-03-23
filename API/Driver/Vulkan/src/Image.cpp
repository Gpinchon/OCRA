#include <OCRA/OCRA.hpp>

#include <VK/Buffer.hpp>
#include <VK/Device.hpp>
#include <VK/Enums.hpp>
#include <VK/Flags.hpp>
#include <VK/Image.hpp>
#include <VK/PhysicalDevice.hpp>
#include <VK/Structs.hpp>

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
        vkCopy.imageExtent = ConvertToVk(ocCopy.imageExtent);
        vkCopy.imageOffset = ConvertToVk(ocCopy.imageOffset);
        vkCopy.imageSubresource = ConvertToVk(ocCopy.imageSubresource);
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
            *srcBuffer,
            *dstImage, vk::ImageLayout::eTransferDstOptimal,
            vkCopies);
        transferCommandBuffer.end();
    }
    {
        vk::SubmitInfo submitInfo;
        submitInfo.setCommandBufferCount(1);
        submitInfo.setPCommandBuffers(&*transferCommandBuffer);
        device.transferQueue.submit({ submitInfo }, nullptr);
    }
    device.transferQueue.waitIdle();
}

void CopyImageToBuffer(
    const OCRA::Buffer::Handle& a_DstBuffer,
    const Image::Handle& a_SrcImage,
    const std::vector<ImageBufferCopy>& a_Regions)
{
    auto& dstBuffer = *a_DstBuffer;
    auto& srcImage = *a_SrcImage;
    auto& device = dstBuffer.device;
    std::vector<vk::BufferImageCopy> vkCopies(a_Regions.size());
    for (auto i = 0u; i < vkCopies.size(); ++i) {
        auto& ocCopy = a_Regions.at(i);
        auto& vkCopy = vkCopies.at(i);
        vkCopy.bufferImageHeight = ocCopy.bufferImageHeight;
        vkCopy.bufferOffset = ocCopy.bufferOffset;
        vkCopy.bufferRowLength = ocCopy.bufferRowLength;
        vkCopy.imageExtent = ConvertToVk(ocCopy.imageExtent);
        vkCopy.imageOffset = ConvertToVk(ocCopy.imageOffset);
        vkCopy.imageSubresource = ConvertToVk(ocCopy.imageSubresource);
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
        transferCommandBuffer.copyImageToBuffer(
            *srcImage, vk::ImageLayout::eTransferDstOptimal,
            *dstBuffer,
            vkCopies);
        transferCommandBuffer.end();
    }
    {
        vk::SubmitInfo submitInfo;
        submitInfo.setCommandBufferCount(1);
        submitInfo.setPCommandBuffers(&*transferCommandBuffer);
        device.transferQueue.submit({ submitInfo }, nullptr);
    }
    device.transferQueue.waitIdle();
}
size_t GetDataSize(const Image::Handle& a_Image)
{
    auto requirements = a_Image->getMemoryRequirements();
    return requirements.size;
}
}
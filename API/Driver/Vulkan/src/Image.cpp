#include <OCRA/OCRA.hpp>

#include <VK/Buffer.hpp>
#include <VK/CommandBuffer.hpp>
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
    const Image::Handle&        a_DstImage,
    const ImageLayout&          a_DstImageLayout,
    const std::vector<ImageBufferCopy>& a_Regions)
{
    auto& srcBuffer = *a_SrcBuffer;
    auto& dstImage = *a_DstImage;
    auto& device = srcBuffer.device;
    ImageSubresourceRange imageRange{};
    imageRange.baseArrayLayer = std::numeric_limits<uint32_t>::max();
    imageRange.baseMipLevel   = std::numeric_limits<uint32_t>::max();
    imageRange.layerCount     = std::numeric_limits<uint32_t>::min();
    imageRange.levelCount     = 1;
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

        imageRange.aspects          |= ocCopy.imageSubresource.aspects;
        imageRange.baseArrayLayer   = std::min(ocCopy.imageSubresource.baseArrayLayer, imageRange.baseArrayLayer);
        imageRange.baseMipLevel     = std::min(ocCopy.imageSubresource.mipLevel,       imageRange.baseMipLevel);
        imageRange.layerCount       = std::max(ocCopy.imageSubresource.layerCount,     imageRange.layerCount);
    }

    Command::Buffer::Handle transferCommandBuffer;
    {
        vk::CommandBufferAllocateInfo commandBufferInfo;
        commandBufferInfo.commandBufferCount = 1;
        commandBufferInfo.commandPool = *device.transferCommandPool;
        commandBufferInfo.level = vk::CommandBufferLevel::ePrimary;
        transferCommandBuffer = std::make_shared<Command::Buffer::Impl>(std::move(device.allocateCommandBuffers(commandBufferInfo).front()), vk::CommandBufferLevel::ePrimary);
    }
    {
        vk::CommandBufferBeginInfo cmdBufferBeginInfo;
        cmdBufferBeginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
        transferCommandBuffer->begin(cmdBufferBeginInfo);
        {
            Command::TransitionImageLayout(
                transferCommandBuffer, { a_DstImage, imageRange,
                ImageLayout::Undefined,
                ImageLayout::TransferDstOptimal });
        }
        transferCommandBuffer->copyBufferToImage(
            *srcBuffer,
            *dstImage, vk::ImageLayout::eTransferDstOptimal,
            vkCopies);
        {
            Command::TransitionImageLayout(
                transferCommandBuffer, { a_DstImage, imageRange,
                ImageLayout::TransferDstOptimal,
                a_DstImageLayout });
        }
        transferCommandBuffer->end();
    }
    {
        vk::SubmitInfo submitInfo;
        submitInfo.setCommandBufferCount(1);
        submitInfo.setPCommandBuffers(&**transferCommandBuffer);
        device.transferQueue.submit({ submitInfo }, nullptr);
    }
    device.transferQueue.waitIdle();
}

void CopyImageToBuffer(
    const OCRA::Buffer::Handle& a_DstBuffer,
    const Image::Handle&        a_SrcImage,
    const ImageLayout&          a_SrcImageLayout,
    const ImageLayout&          a_DstImageLayout,
    const std::vector<ImageBufferCopy>& a_Regions)
{
    auto& dstBuffer = *a_DstBuffer;
    auto& srcImage = *a_SrcImage;
    auto& device = dstBuffer.device;
    ImageSubresourceRange imageRange;
    imageRange.baseArrayLayer = std::numeric_limits<uint32_t>::max();
    imageRange.baseMipLevel   = std::numeric_limits<uint32_t>::max();
    imageRange.layerCount     = std::numeric_limits<uint32_t>::min();
    imageRange.levelCount     = 1;
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

        imageRange.aspects          |= ocCopy.imageSubresource.aspects;
        imageRange.baseArrayLayer   = std::min(ocCopy.imageSubresource.baseArrayLayer, imageRange.baseArrayLayer);
        imageRange.baseMipLevel     = std::min(ocCopy.imageSubresource.mipLevel,       imageRange.baseMipLevel);
        imageRange.layerCount       = std::max(ocCopy.imageSubresource.layerCount,     imageRange.layerCount);
    }
    Command::Buffer::Handle transferCommandBuffer;
    {
        vk::CommandBufferAllocateInfo commandBufferInfo;
        commandBufferInfo.commandBufferCount = 1;
        commandBufferInfo.commandPool = *device.transferCommandPool;
        commandBufferInfo.level = vk::CommandBufferLevel::ePrimary;
        transferCommandBuffer = std::make_shared<Command::Buffer::Impl>(std::move(device.allocateCommandBuffers(commandBufferInfo).front()), vk::CommandBufferLevel::ePrimary);
    }
    {
        vk::CommandBufferBeginInfo cmdBufferBeginInfo;
        cmdBufferBeginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
        transferCommandBuffer->begin(cmdBufferBeginInfo);
        {
            Command::TransitionImageLayout(
                transferCommandBuffer,
                { a_SrcImage, imageRange,
                a_SrcImageLayout,
                ImageLayout::TransferSrcOptimal });
        }
        transferCommandBuffer->copyImageToBuffer(
            *srcImage, vk::ImageLayout::eTransferSrcOptimal,
            *dstBuffer,
            vkCopies);
        {
            Command::TransitionImageLayout(
                transferCommandBuffer,
                { a_SrcImage, imageRange,
                ImageLayout::TransferSrcOptimal,
                a_DstImageLayout });
        }
        transferCommandBuffer->end();
    }
    {
        vk::SubmitInfo submitInfo;
        submitInfo.setCommandBufferCount(1);
        submitInfo.setPCommandBuffers(&**transferCommandBuffer);
        device.transferQueue.submit({ submitInfo }, nullptr);
    }
    device.transferQueue.waitIdle();
}

void TransitionLayouts(
    const std::vector<ImageLayoutTransitionInfo>& a_Transitions)
{
    auto& device = a_Transitions.front().image->device;

    vk::raii::CommandBuffer transitionCommandBuffer = nullptr;
    {
        vk::CommandBufferAllocateInfo commandBufferInfo;
        commandBufferInfo.commandBufferCount = 1;
        commandBufferInfo.commandPool = *device.transferCommandPool;
        commandBufferInfo.level = vk::CommandBufferLevel::ePrimary;
        transitionCommandBuffer = std::move(device.allocateCommandBuffers(commandBufferInfo).front());
    }
    vk::PipelineStageFlags vkSrcStageFlags;
    vk::PipelineStageFlags vkDstStageFlags;
    std::vector<vk::ImageMemoryBarrier> barriers(a_Transitions.size());
    for (auto i = 0u; i < barriers.size(); ++i) {
        auto& transition = a_Transitions.at(i);
        auto& image = *transition.image;
        auto& subResource = transition.subRange;
        auto& srcQueueFamily = transition.srcQueueFamilyIndex;
        auto& dstQueueFamily = transition.dstQueueFamilyIndex;
        auto& barrier = barriers.at(i);
        auto oldLayout = ConvertToVk(transition.oldLayout);
        auto newLayout = ConvertToVk(transition.newLayout);
        vkSrcStageFlags |= GetImageTransitionStage(oldLayout);
        vkDstStageFlags |= GetImageTransitionStage(newLayout);
        barrier.image = *image;
        barrier.subresourceRange = ConvertToVk(subResource);
        barrier.oldLayout = oldLayout;
        barrier.newLayout = newLayout;
        barrier.srcQueueFamilyIndex = srcQueueFamily;
        barrier.dstQueueFamilyIndex = dstQueueFamily;
        barrier.srcAccessMask = GetImageTransitionAccessMask(oldLayout);
        barrier.dstAccessMask = GetImageTransitionAccessMask(newLayout);
    }
    {
        vk::CommandBufferBeginInfo cmdBufferBeginInfo;
        cmdBufferBeginInfo.flags = vk::CommandBufferUsageFlagBits::eOneTimeSubmit;
        transitionCommandBuffer.begin(cmdBufferBeginInfo);
        transitionCommandBuffer.pipelineBarrier(
            vkSrcStageFlags, vkDstStageFlags,
            vk::DependencyFlags{},
            {}, {}, barriers);
        transitionCommandBuffer.end();
    }
    device.transferQueue.waitIdle();
}

size_t GetDataSize(const Image::Handle& a_Image)
{
    auto requirements = a_Image->getMemoryRequirements();
    return requirements.size;
}
}
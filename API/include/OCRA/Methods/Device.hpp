/*
 * @Author: gpinchon
 * @Date:   2021-09-26 00:00:00
 * @Last Modified by:   gpinchon
 * @Last Modified time: 2021-09-26 14:26:36
 */
#pragma once

#include <OCRA/Structs.hpp>

namespace OCRA::Device {
/**
 * @brief Creates an empty buffer, unuseable without binding Memory to it
 */
Buffer::Handle CreateBuffer(
    const Handle& a_Device,
    const CreateBufferInfo& a_Info,
    const AllocationCallback* a_Allocator = nullptr);
/**
 * @brief Allocates a memory using the specified memory flags,
 * creates a new buffer and binds the memory to it
 */
Buffer::Handle AllocateBuffer(
    const Handle& a_Device,
    const AllocateBufferInfo& a_Info,
    const AllocationCallback* a_Allocator = nullptr);

Command::Pool::Handle CreateCommandPool(
    const Device::Handle& a_Device,
    const CreateCommandPoolInfo& a_Info,
    const AllocationCallback* a_Allocator = nullptr);

/**
 * Creates a descriptor pool with specified size
 * Pools will always enable for modification after binding and freeing sets
 * In Vulkan, pools will always have the flags FREE_DESCRIPTOR_SET and UPDATE_AFTER_BIND
 */
Descriptor::Pool::Handle CreateDescriptorPool(
    const Device::Handle& a_Device,
    const CreateDescriptorPoolInfo& a_Info,
    const AllocationCallback* a_Allocator = nullptr);

/**
 * @brief Creates a Fence with the specified default status (optional)
 */
Fence::Handle CreateFence(
    const Handle& a_Device,
    const FenceStatus& a_DefaultStatus    = FenceStatus::Unsignaled,
    const AllocationCallback* a_Allocator = nullptr);

Image::Handle CreateImage(
    const Device::Handle& a_Device,
    const CreateImageInfo& a_Info);

Image::Sampler::Handle CreateImageSampler(
    const Device::Handle& a_Device,
    const CreateImageSamplerInfo& a_Info);

Image::View::Handle CreateImageView(
    const Device::Handle& a_Device,
    const CreateImageViewInfo& a_Info);

Pipeline::Handle CreatePipelineGraphics(
    const Device::Handle& a_Device,
    const CreatePipelineGraphicsInfo& a_Info);

Handle CreateQueryPool(
    const Device::Handle& a_Device,
    const CreateQueryPoolInfo& a_Info);

Semaphore::Handle CreateSemaphore(
    const Device::Handle& a_Device,
    const CreateSemaphoreInfo& a_Info,
    const AllocationCallback* a_Allocator = nullptr);

ShaderModule::Handle CreateShaderModule(
    const Device::Handle& a_Device,
    const CreateShaderModuleInfo& a_Info);

/**
 * @brief on OpenGL, there is only one queue
 * @return the handle to a que of specified Family.
 * To get family index, use GetQueueFamilyProperties
 */
Queue::Handle GetQueue(
    const Handle& a_Device,
    uint32_t a_FamilyIndex,
    uint32_t a_QueueIndex);

/**
 * @brief Allocates a buffer on device's memory
 */
Memory::Handle AllocateMemory(
    const Handle& a_Device,
    const AllocateMemoryInfo& a_Info);
/**
 * @brief Finds an appropriate memory type and allocates it
 */
Memory::Handle AllocateMemory(
    const Handle& a_Device,
    const size_t a_Size,
    const MemoryPropertyFlags& a_MemoryFlags);

/**
 * Wait for the device to finish commands execution
 * In OGL, this will push an empty synchronized command
 */
void WaitIdle(const Handle& a_Device);

/**
* Clears the caches of this device
* Usually this includes pipeline, pipeline layout, descriptor set layout caches
* WARNING : if the objects stored in cache are in use, this might cause undefined behavior
* In general, this function should only be used after releasing all objects from device
*/
void ClearCache(const Handle& a_Device);
}

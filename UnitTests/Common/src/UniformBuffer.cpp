#include <UniformBuffer.hpp>

#include <OCRA/OCRA.hpp>

namespace OCRA {
bool UniformBuffer::Update() {
    bool updated = false;
    if (dirtyDescriptorSet) {
        DescriptorSetBufferInfo info;
        info.buffer = buffer;
        info.offset = 0;
        info.range = data.size();
        std::vector<DescriptorSetWrite> writeOperations(1);
        DescriptorSetBufferInfo bufferInfo;
        bufferInfo.buffer = buffer;
        bufferInfo.offset = 0;
        bufferInfo.range = data.size();
        writeOperations.front().dstCount = 1;
        writeOperations.front().bufferInfo = bufferInfo;
        writeOperations.front().dstBinding = GetDescriptorSetLayoutBindings().front().binding;
        writeOperations.front().type = DescriptorType::UniformBuffer;
        SetWriteOperations(writeOperations);
        dirtyDescriptorSet = false;
        updated = true;
    }
    if (dirtyData) {
        MemoryMappedRange range;
        range.length = data.size();
        range.memory = memory;
        auto ptr = Memory::Map(range);
        std::memcpy(ptr, data.data(), data.size());
        Memory::Unmap(memory);
        dirtyData = false;
        updated = true;
    }
    return updated;
}
}
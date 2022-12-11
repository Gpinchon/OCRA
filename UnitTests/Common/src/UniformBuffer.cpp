#include <UniformBuffer.hpp>

#include <OCRA/Descriptor/Set.hpp>
#include <OCRA/Memory.hpp>

namespace OCRA {
void UniformBuffer::Update() {
    if (dirtyDescriptorSet) {
        Descriptor::Set::BufferInfo info;
        info.buffer = buffer;
        info.offset = 0;
        info.range = data.size();
        std::vector<Descriptor::Set::WriteOperation> writeOperations(1);
        Descriptor::Set::BufferInfo bufferInfo;
        bufferInfo.buffer = buffer;
        bufferInfo.offset = 0;
        bufferInfo.range = data.size();
        writeOperations.front().bufferInfo = bufferInfo;
        writeOperations.front().dstSet = GetDescriptorSets().front();
        writeOperations.front().dstBinding = GetDescriptorSetLayoutBindings().front().binding;
        writeOperations.front().type = Descriptor::Type::UniformBuffer;
        Descriptor::Set::Update(GetDevice(), writeOperations, {});
        dirtyDescriptorSet = false;
    }
    if (dirtyData) {
        Memory::MappedRange range;
        range.length = data.size();
        range.memory = memory;
        auto ptr = Memory::Map(GetDevice(), range);
        std::memcpy(ptr, data.data(), data.size());
        Memory::Unmap(GetDevice(), memory);
        dirtyData = false;
    }
}
}
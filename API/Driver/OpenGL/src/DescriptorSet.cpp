#include <OCRA/Core.hpp>

#include <GL/DescriptorSet.hpp>

namespace OCRA::Descriptor::Set {
void Update(
    const std::vector<DescriptorSetWrite>&  a_Writes,
    const std::vector<DescriptorSetCopy>&   a_Copies)
{
    for (const auto& writeOperation : a_Writes) {
        writeOperation.dstSet->Write(writeOperation);
    }
    for (const auto& copyOperation : a_Copies) {
        copyOperation.dstSet->Copy(copyOperation);
    }
}
}

#include <OCRA/Descriptor/Set.hpp>

#include <GL/Descriptor/Set.hpp>

namespace OCRA::Descriptor::Set {
void Update(
    const Device::Handle& a_Device,
    const std::vector<WriteOperation>&  a_Writes,
    const std::vector<CopyOperation>&   a_Copies)
{
    for (const auto& writeOperation : a_Writes) {
        writeOperation.dstSet->Write(writeOperation);
    }
    for (const auto& copyOperation : a_Copies) {
        copyOperation.dstSet->Copy(copyOperation);
    }
}
}

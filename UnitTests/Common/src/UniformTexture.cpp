#include <UniformTexture.hpp>

namespace OCRA {
void UniformTexture::Update()
{
    if (!dirty) return;
    Descriptor::Set::ImageInfo setImageInfo;
    Descriptor::Set::WriteOperation writeOp;
    setImageInfo.imageView = texture.GetImageView();
    setImageInfo.sampler = texture.GetSampler();
    writeOp.type = Descriptor::Type::SampledImage;
    writeOp.imageInfo = setImageInfo;
    writeOp.dstBinding = GetDescriptorSetLayoutBindings().front().binding;
    SetWriteOperations({ writeOp });
    dirty = false;
}

Descriptor::SetLayout::Binding UniformTexture::CreateSetLayoutBinding(const uint32_t a_Binding) {
    Descriptor::SetLayout::Binding binding;
    binding.binding = a_Binding;
    binding.count = 1;
    binding.type = Descriptor::Type::SampledImage;
    return binding;
}
}

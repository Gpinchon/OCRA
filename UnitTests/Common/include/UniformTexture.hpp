#pragma once

#include <Texture.hpp>
#include <Uniform.hpp>

#include <Handle.hpp>

OCRA_DECLARE_HANDLE(OCRA::Device);

namespace OCRA {
class UniformTexture : public Uniform {
public:
	UniformTexture(const uint32_t& a_Binding, const Device::Handle& a_Device, const Texture& a_Texture)
        : Uniform({ CreateSetLayoutBinding(a_Binding) }, a_Device)
        , texture(a_Texture)
    {}
    void SetTexture(const Texture& a_Texture) {
        texture = a_Texture;
        dirty = true;
    }
    auto& GetTexture() const { return texture; }
    virtual void Update() override;
private:
    static Descriptor::SetLayout::Binding CreateSetLayoutBinding(const uint32_t a_Binding);
    bool    dirty{ true };
    Texture texture;
};
}
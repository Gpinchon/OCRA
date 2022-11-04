#pragma once

#include <Texture.hpp>
#include <Uniform.hpp>

#include <Handle.hpp>

OCRA_DECLARE_HANDLE(OCRA::Device);

namespace OCRA {
class UniformTexture : public Uniform {
public:
    UniformTexture() = default;
    UniformTexture(const UniformTexture&) = default;
	UniformTexture(const uint32_t& a_Binding, const Device::Handle& a_Device, const Descriptor::Pool::Handle& a_Pool, const Texture& a_Texture)
        : Uniform({ CreateSetLayoutBinding(a_Binding) }, a_Device, a_Pool)
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
    Texture texture{};
};
}
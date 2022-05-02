#include <GL/Surface.hpp>

namespace OCRA::Surface::Win32
{
struct Impl : Surface::Impl
{
    Impl(const Instance::Handle& a_Instance, const Info& a_Info);
    ~Impl();
    virtual uiExtent2D GetExtent() override;
    const void* hdc;
};
}
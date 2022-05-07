#include <GL/Surface.hpp>

namespace OCRA::Surface::Win32
{
struct Impl : Surface::Impl
{
    Impl(const Instance::Handle& a_Instance, const Info& a_Info);
    ~Impl();
    const void* hdc;
};
}
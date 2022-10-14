#include <GL/Image/Format.hpp>

namespace OCRA::Image {
size_t GetPixelSize(const Format& a_Format)
{
    return
        GetRedSize(a_Format) + GetGreenSize(a_Format) +
        GetRedSize(a_Format) + GetAlphaSize(a_Format) +
        GetDepthSize(a_Format) + GetStencilSize(a_Format);
}
}
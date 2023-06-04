#pragma once

namespace OCRA::OpenGL::Win32 {
void Initialize();
void* CreateContext(const void* a_HDC);
void SetDefaultPixelFormat(const void* a_HDC);
}

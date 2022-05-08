
# OCRA
OCRA stands for Overly Complicated Rendering Abstraction

It aims to give a common abstraction between Vulkan, OpenGL and DirectX using Vulkan data structure, or something close to it.

This adds command buffers, semaphores, fences and queues for OpenGL, making multithreading easier.

This project allows for easier ports over various APIs and easier state management for OGL.

## OpenGL
For OGL, it will "compile" the states into lambda calls and give the ability to replay previously created graphics pipelines. This will make creation of programable pipelines and multi-threading in 3D Engine easier.

In this mode, OpenGL is run from a thread and commands are being pushed to it. It never actually renders anything to your window, presenting is done through SwapChain::Present, which uses D3D10 and [WGL_NV_DX_interop](https://www.khronos.org/registry/OpenGL/extensions/NV/WGL_NV_DX_interop.txt)

## Vulkan
**NOT YET SUPPORTED**

When used with Vulkan, it will simply operate like an interface with Vulkan's functions.

## DirectX
**NOT YET SUPPORTED**

## Dependencies
### Shared
 - Latest build of [CMake](https://cmake.org/download/)

### Opengl backend
 - [GLEW-CMake](https://github.com/Perlmint/glew-cmake.git) (Fetched by CMake and compiled automatically)
 - [DirectX SDK](https://docs.microsoft.com/fr-fr/windows/win32/directx-sdk--august-2009-)

## Compilation
Simply run CMake and select the backend you want to use as RenderApi

## Debugging
In order to use [NVIDIA Nsight Graphics](https://developer.nvidia.com/nsight-graphics) with OCRA, you must enable the USE_D3D11 tickbox in CMake, this will use the D3D11 implementation of the SwapChain and push a glFinish after presentation to delimite frames.

D3D11 SwapChain won't give you any performance advantage and has been added solely for debugging purpose.

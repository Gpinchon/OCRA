## OCRA

OCRA stands for Overly Complicated Rendering Abstraction

It aims to give a common abstraction between Vulkan, OpenGL and DirectX using Vulkan data structure, or something close to it

It features Graphics Pipeline and a whole bunch of fixed pipeline states to go with it.

# OpenGL
For OGL, it will "compile" the states into lambda calls and give the ability to replay previously created graphics pipelines.
This will make creation of programable pipelines and multi-threading in 3D Engine easier.

This project allows for easier ports over various APIs and easier state management for OGL.
When used with Vulkan, it will simply operate like an interface with Vulkan's functions.
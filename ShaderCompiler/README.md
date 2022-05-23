
# OCRA Shader Compiler
This is the shader compiler. This will do some reflection on your glsl shader code in order to make it compatible with the API abstraction.

## How it works
It takes glsl human-readable code and compiles it to SPIR-V using GLSLang.

This blob is then passed to SPIRV-Cross in order to do some reflection on it.

Finally the resulting glsl code is compiled once again using GLSLang.
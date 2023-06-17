include(FetchContent)

# Fetch SPIRV_CROSS
macro(Fetch_SPIRVCross)
  FetchContent_Declare(
    SPIRV_CROSS
    GIT_REPOSITORY  https://github.com/KhronosGroup/SPIRV-Cross.git
    GIT_TAG         sdk-1.3.239.0
  )
  option(SPIRV_CROSS_SKIP_INSTALL "Skip SPIRV-Cross Installation" ON)
  option(SPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS "Instead of throwing exceptions assert" OFF)
  option(SPIRV_CROSS_SHARED "Build the C API as a single shared library." OFF)
  option(SPIRV_CROSS_STATIC "Build the C and C++ API as static libraries." ON)
  option(SPIRV_CROSS_CLI "Build the CLI binary. Requires SPIRV_CROSS_STATIC." OFF)
  option(SPIRV_CROSS_ENABLE_TESTS "Enable SPIRV-Cross tests." OFF)
  option(SPIRV_CROSS_ENABLE_GLSL "Enable GLSL support." ON)
  option(SPIRV_CROSS_ENABLE_HLSL "Enable HLSL target support." ON)
  option(SPIRV_CROSS_ENABLE_MSL "Enable MSL target support." ON)
  option(SPIRV_CROSS_ENABLE_CPP "Enable C++ target support." ON)
  option(SPIRV_CROSS_ENABLE_REFLECT "Enable JSON reflection target support." ON)
  option(SPIRV_CROSS_ENABLE_C_API "Enable C API wrapper support in static library." OFF)
  option(SPIRV_CROSS_ENABLE_UTIL "Enable util module support." OFF)
  FetchContent_MakeAvailable(SPIRV_CROSS)
  set_subdirectory_folder("3rdparty/SPIRV-Cross" ${spirv_cross_SOURCE_DIR})
endmacro()
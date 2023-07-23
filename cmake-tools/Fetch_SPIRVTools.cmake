include(FetchContent)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake-tools/Fetch_SPIRVHeaders.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake-tools/Fetch_GoogleTest.cmake)

#Fetch SPV-Tools
macro(Fetch_SPIRVTools)
  #try finding the package first
  find_package(SPIRV-Tools CONFIG QUIET)
  if (NOT TARGET SPIRV-Tools-static)
    Fetch_SPIRVHeaders()
    Fetch_GoogleTest()
    FetchContent_Declare(
      SPIRV_TOOLS
      GIT_REPOSITORY  https://github.com/KhronosGroup/SPIRV-Tools.git
      GIT_TAG         sdk-1.3.250.1)
    FetchContent_GetProperties(SPIRV_TOOLS)
    if (NOT spirv_tools_POPULATED)
      FetchContent_Populate(SPIRV_TOOLS)    
      message("SPV-Tools fetched to ${spirv_tools_SOURCE_DIR}")
    endif (NOT spirv_tools_POPULATED)
    set(SPIRV_BUILD_TYPE Release)
    execute_process(
      COMMAND ${CMAKE_COMMAND}
        "-DCMAKE_BUILD_TYPE=Release"
        "-DSPIRV-Headers_SOURCE_DIR=${SPIRV-Headers_SOURCE_DIR}"
        -S ${spirv_tools_SOURCE_DIR}
        -B ${spirv_tools_BINARY_DIR})
    execute_process(
      COMMAND ${CMAKE_COMMAND}
        --build ${spirv_tools_BINARY_DIR}
        --config Release)
    execute_process(
      COMMAND ${CMAKE_COMMAND}
        --install ${spirv_tools_BINARY_DIR}
        --prefix ${OCRA_EXTERNAL_PATH}
        --config Release)
    find_package(SPIRV-Tools REQUIRED CONFIG QUIET)
    message("SPV-Tools built to ${OCRA_EXTERNAL_PATH}")
  endif(NOT TARGET SPIRV-Tools-static)
endmacro()

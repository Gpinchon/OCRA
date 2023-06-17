include(FetchContent)

#Fetch SPIRV-Headers
macro(Fetch_SPIRVHeaders)
    FetchContent_Declare(
      SPIRV_HEADERS
      GIT_REPOSITORY  https://github.com/KhronosGroup/SPIRV-Headers.git
      GIT_TAG         main)
    FetchContent_GetProperties(SPIRV_HEADERS)
    if (NOT spirv_headers_POPULATED)
      FetchContent_Populate(SPIRV_HEADERS)
      message("SPV-Headers fetched to ${spirv_headers_SOURCE_DIR}")
    endif (NOT spirv_headers_POPULATED)
  find_package(SPIRV-Headers CONFIG QUIET)
  if(NOT TARGET SPIRV-Headers::SPIRV-Headers)
    execute_process(
      COMMAND ${CMAKE_COMMAND}
        "-DCMAKE_BUILD_TYPE=Release"
        -S ${spirv_headers_SOURCE_DIR}
        -B ${spirv_headers_BINARY_DIR})
    execute_process(
      COMMAND ${CMAKE_COMMAND}
        --install ${spirv_headers_BINARY_DIR}
        --config Release
        --prefix ${OCRA_EXTERNAL_PATH})
    find_package(SPIRV-Headers REQUIRED CONFIG QUIET)
    message("SPV-Headers built to ${OCRA_EXTERNAL_PATH}")
    set(SPIRV-Headers_SOURCE_DIR "${spirv_headers_SOURCE_DIR}" CACHE PATH "Home of SPIRV Header source")
  endif(NOT TARGET SPIRV-Headers::SPIRV-Headers)
endmacro()

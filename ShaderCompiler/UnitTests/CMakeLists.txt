cmake_minimum_required(VERSION 3.19)
cmake_policy(SET CMP0074 NEW)

include(../../cmake-tools/GroupSources.cmake)

set(OCRA_TEST_SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
set(OCRA_TEST_HEADER_FILES
  )
add_executable(OCRA-ShaderCompiler-UnitTests
  ${OCRA_TEST_SOURCE_FILES}
  ${OCRA_TEST_HEADER_FILES})
target_include_directories(OCRA-ShaderCompiler-UnitTests PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_link_libraries(OCRA-ShaderCompiler-UnitTests OCRA::ShaderCompiler::API)

if(MSVC_IDE)
  # Run macro
  GroupSources(src)
  GroupSources(include)
endif()
cmake_minimum_required(VERSION 3.19)
cmake_policy(SET CMP0074 NEW)

Fetch_GoogleTest()

set(OCRA_TEST_SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
add_executable(OCRA-UnitTests-ShaderCompiler
  ${OCRA_TEST_SOURCE_FILES})

target_link_libraries(
  OCRA-UnitTests-ShaderCompiler PRIVATE
    GTest::GTest
    OCRA::ShaderCompiler::API)

add_test(ShaderCompiler OCRA-UnitTests-ShaderCompiler)

set_target_properties(
  OCRA-UnitTests-ShaderCompiler
  PROPERTIES FOLDER "ShaderCompiler"
)

if(MSVC_IDE)
  # Run macro
  GroupSources(src)
  GroupSources(include)
endif()
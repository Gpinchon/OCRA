cmake_minimum_required(VERSION 3.19)
cmake_policy(SET CMP0074 NEW)

Fetch_GoogleTest()

set(OCRA_TEST_COMMAND_BUFFER_SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
add_executable(OCRA-UnitTests-DescriptorSets
  ${OCRA_TEST_COMMAND_BUFFER_SOURCE_FILES})
target_include_directories(OCRA-UnitTests-DescriptorSets PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/include")

if(MSVC_IDE)
  set_property(TARGET OCRA-UnitTests-DescriptorSets PROPERTY
    VS_DEBUGGER_ENVIRONMENT "${OCRA_DEBUGGER_ENV}")
endif()

target_link_libraries(
  OCRA-UnitTests-DescriptorSets PRIVATE
    GTest::GTest
    OCRA-UnitTests-Lib-Common
    OCRA::API::Interface
    OCRA::API::Implementation)

add_test(DescriptorSetTests OCRA-UnitTests-DescriptorSets)

if(MSVC_IDE)
  # Run macro
  GroupSources(src)
  GroupSources(include)
endif()
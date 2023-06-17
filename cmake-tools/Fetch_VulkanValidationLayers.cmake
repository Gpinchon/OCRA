include(FetchContent)
include(cmake-tools/Fetch_VulkanHeaders.cmake)
include(cmake-tools/Fetch_SPIRVHeaders.cmake)
include(cmake-tools/Fetch_SPIRVTools.cmake)

#Fetch Vulkan-ValidationLayers
macro(Fetch_VulkanValidationLayers)
  if (NOT DEFINED OCRA_VK_LAYER_PATH)
    Fetch_VulkanHeaders()
    Fetch_SPIRVHeaders()
    Fetch_SPIRVTools()
    FetchContent_Declare(
      VK_LAYERS
      GIT_REPOSITORY  https://github.com/KhronosGroup/Vulkan-ValidationLayers.git
      GIT_TAG         v1.3.244)
    FetchContent_GetProperties(VK_LAYERS)
    if (NOT vk_layers_POPULATED)
      FetchContent_Populate(VK_LAYERS)
      message("Vulkan-ValidationLayers fetched to ${vk_layers_SOURCE_DIR}")
    endif (NOT vk_layers_POPULATED)
    execute_process(COMMAND ${CMAKE_COMMAND} "-DCMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}" -S ${vk_layers_SOURCE_DIR} -B ${vk_layers_BINARY_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND} --build ${vk_layers_BINARY_DIR} --config Release)
    execute_process(COMMAND ${CMAKE_COMMAND} --install ${vk_layers_BINARY_DIR} --config Release --prefix ${OCRA_EXTERNAL_PATH})
    set(OCRA_VK_LAYER_PATH "${OCRA_EXTERNAL_PATH}/bin" CACHE PATH "Home of Vulkan layers")
    set(OCRA_DEBUGGER_ENV "VK_LAYER_PATH=${OCRA_VK_LAYER_PATH}" CACHE STRING "The debugging environment")
    message("Vulkan-ValidationLayers built to ${OCRA_EXTERNAL_PATH}")
  endif (NOT DEFINED OCRA_VK_LAYER_PATH)
endmacro()
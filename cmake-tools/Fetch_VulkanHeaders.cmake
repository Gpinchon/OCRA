include(FetchContent)

#Fetch Vulkan-Headers
macro(Fetch_VulkanHeaders)
  find_package(VulkanHeaders CONFIG QUIET)
  if (NOT TARGET Vulkan::Headers)
    FetchContent_Declare(
      VK_HEADERS
      GIT_REPOSITORY  https://github.com/KhronosGroup/Vulkan-Headers.git
      GIT_TAG         v1.3.244)
    FetchContent_GetProperties(VK_HEADERS)
    if (NOT vk_headers_POPULATED)
      FetchContent_Populate(VK_HEADERS)
      message("Vulkan-Headers fetched to ${vk_headers_SOURCE_DIR}")
    endif (NOT vk_headers_POPULATED)
  
    execute_process(COMMAND ${CMAKE_COMMAND} -S ${vk_headers_SOURCE_DIR} -B ${vk_headers_BINARY_DIR})
    execute_process(COMMAND ${CMAKE_COMMAND} --install ${vk_headers_BINARY_DIR} --prefix ${OCRA_EXTERNAL_PATH})
    find_package(VulkanHeaders REQUIRED CONFIG QUIET)
    message("Vulkan-Headers built to ${OCRA_EXTERNAL_PATH}")
  endif (NOT TARGET Vulkan::Headers)
endmacro()

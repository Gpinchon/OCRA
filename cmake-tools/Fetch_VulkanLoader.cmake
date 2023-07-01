include(FetchContent)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake-tools/Fetch_GoogleTest.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake-tools/Fetch_VulkanHeaders.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake-tools/Fetch_SPIRVTools.cmake)

#Fetch Vulkan-Loader
macro(Fetch_VulkanLoader)
  Fetch_GoogleTest()
  Fetch_VulkanHeaders()
  Fetch_SPIRVTools()
  FetchContent_Declare(
    VK_LOADER
    GIT_REPOSITORY  https://github.com/KhronosGroup/Vulkan-Loader.git
    GIT_TAG         v1.3.244
  )
  FetchContent_MakeAvailable(VK_LOADER)
  set_subdirectory_folder("3rdparty/Vulkan-Loader" ${vk_loader_SOURCE_DIR})
  message("Vulkan-Loader fetched to ${vk_loader_SOURCE_DIR}")
endmacro()
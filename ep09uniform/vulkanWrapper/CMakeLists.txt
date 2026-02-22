file(GLOB_RECURSE VULKAN ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(vulkanLib STATIC ${VULKAN})

target_link_libraries(vulkanLib Vulkan::Vulkan)
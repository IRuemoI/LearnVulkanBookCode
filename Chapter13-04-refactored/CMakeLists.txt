# 设置当前章节的名称
get_filename_component(CURRENT_FOLDER_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(CHAPTER_NAME ${CURRENT_FOLDER_NAME})

# ---- Include guards ----
if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
            FATAL_ERROR
            "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
    )
endif ()

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/${CHAPTER_NAME}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/${CHAPTER_NAME})
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/${CHAPTER_NAME}/lib)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/dependencies/glm
        ${PROJECT_SOURCE_DIR}/dependencies/glfw/include
        ${VULKAN_PATH}/Include
)

aux_source_directory(. DIRSRCS)

add_subdirectory(vulkanWrapper)
add_subdirectory(texture)

# 创建一个可执行文件
add_executable(${CHAPTER_NAME} ${DIRSRCS})

target_link_libraries(${CHAPTER_NAME}
        textureLibRefactored
        vulkanLibRefactored
        Vulkan::Vulkan
        glslang
        glslang::SPIRV
        glslang::glslang-default-resource-limits
        glfw
        glm
)

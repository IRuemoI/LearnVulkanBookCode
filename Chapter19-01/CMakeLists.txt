# 设置当前章节的名称
set(CHAPTER_NAME Chapter19-01)

# ---- Include guards ----
if (PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
            FATAL_ERROR
            "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
    )
endif ()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

# 查找当前目录下所有源文件和头文件，并将它们分别存入变量中
file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/${CHAPTER_NAME}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${PROJECT_SOURCE_DIR}/${CHAPTER_NAME}/src/*.h")

# 创建一个可执行文件
add_executable(${CHAPTER_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${CHAPTER_NAME}
        PRIVATE
        ${CMAKE_SOURCE_DIR}/${CHAPTER_NAME}
)

target_link_libraries(${CHAPTER_NAME}
        Vulkan::Vulkan
        glslang
        glslang::SPIRV
        glslang::glslang-default-resource-limits
        glfw
        SDL2::SDL2-static
        SDL2_mixer::SDL2_mixer-static
)

# 复制 assets 文件夹到构建目录
add_custom_command(TARGET ${CHAPTER_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/assets"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/assets"
                "${CMAKE_CURRENT_BINARY_DIR}/assets"
        COMMENT "Copying assets to build directory..."
)

# 复制 shaders 文件夹到构建目录
add_custom_command(TARGET ${CHAPTER_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/shaders"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/shaders"
                "${CMAKE_CURRENT_BINARY_DIR}/shaders"
        COMMENT "Copying shaders to build directory..."
)

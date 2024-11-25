#include "ShaderCompileUtil.h"
#include "util.h"
#include <glslang/Public/ShaderLang.h>
#include <iomanip>

// 服务于内部使用的类型映射方法
struct shader_type_mapping
{
    VkShaderStageFlagBits vkshader_type;
    shaderc_shader_kind shaderc_type;
};
// 服务于内部使用的类型映射方法
static const shader_type_mapping shader_map_table[] = {
        {VK_SHADER_STAGE_VERTEX_BIT, shaderc_glsl_vertex_shader},
        {VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, shaderc_glsl_tess_control_shader},
        {VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, shaderc_glsl_tess_evaluation_shader},
        {VK_SHADER_STAGE_GEOMETRY_BIT, shaderc_glsl_geometry_shader},
        {VK_SHADER_STAGE_FRAGMENT_BIT, shaderc_glsl_fragment_shader},
        {VK_SHADER_STAGE_COMPUTE_BIT, shaderc_glsl_compute_shader},
};

// 内部使用的类型映射方法
shaderc_shader_kind MapShadercType(VkShaderStageFlagBits vkShader) {
    for (auto shader: shader_map_table) {
        if (shader.vkshader_type == vkShader) {
            return shader.shaderc_type;
        }
    }
    return shaderc_glsl_infer_from_source;
}

bool compileGLSLtoSPV(const VkShaderStageFlagBits shader_type, const char *pshader, std::vector<uint32_t> &spirv) {
    glslang::InitializeProcess();

    EShLanguage stage = FindLanguage(shader_type);
    glslang::TShader shader(stage);
    shader.setStrings(&pshader, 1);
    shader.setEnvInput(glslang::EShSourceGlsl, stage, glslang::EShClientVulkan, 450);
    shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_2);
    shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_5);

    TBuiltInResource Resources;
    init_resources(Resources);
    // Enable SPIR-V and Vulkan rules when parsing GLSL
    // EShMessages messages = (EShMessages) (EShMsgSpvRules | EShMsgVulkanRules);

    if (!shader.parse(&Resources, 450, ENoProfile, false, false, EShMsgDefault)) {
        puts(shader.getInfoLog());
        puts(shader.getInfoDebugLog());
        return false;// something didn't work
    }

    glslang::TProgram program;
    program.addShader(&shader);

    // Program-level processing...

    if (!program.link(EShMsgDefault)) {
        puts(shader.getInfoLog());
        puts(shader.getInfoDebugLog());
        fflush(stdout);
        return false;
    }

    glslang::GlslangToSpv(*program.getIntermediate(stage), spirv);

    glslang::FinalizeProcess();

    return true;
}
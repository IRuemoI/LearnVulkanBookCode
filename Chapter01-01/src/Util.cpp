#include "Util.h"

using namespace std;

int preX = 0;
int preY = 0;
bool left_mouse_down = false;

void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    VulkanDemoApp::currentCursorX = xpos;
    VulkanDemoApp::currentCursorY = ypos;

    // 处理鼠标拖动事件
    if (left_mouse_down)
    {
        VulkanDemoApp::cursorDeltaX = preX - VulkanDemoApp::currentCursorX;
        VulkanDemoApp::cursorDeltaY = preY - VulkanDemoApp::currentCursorY;
        VulkanDemoApp::yAngle = VulkanDemoApp::yAngle + (float)VulkanDemoApp::cursorDeltaX * 180 / 200;
        VulkanDemoApp::zAngle = VulkanDemoApp::zAngle + (float)VulkanDemoApp::cursorDeltaY * 180 / 200;
        printf("拖动鼠标的表示的旋转:[Y轴:%f,Z轴:%f]\n", VulkanDemoApp::yAngle, VulkanDemoApp::zAngle);
        //printf("拖动鼠标的变化量:[X:%f,Y:%f]\n", VulkanAdvanceApplication::cursorDeltaX, VulkanAdvanceApplication::cursorDeltaY);
    }

    //保存本次的鼠标位置作为上次的位置
    preX = (int)xpos;
    preY = (int)ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        left_mouse_down = true;
        printf("鼠标按下\n");
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        left_mouse_down = false;
        printf("鼠标松开\n");
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    printf("scroll called [%F %F]\n", xoffset, yoffset);
}

// 键盘按键回调函数
void processInputs(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        std::cout << "按下了W键" << std::endl;
}

/**
 * 初始化资源限制
 *
 * @param Resources TBuiltInResource类型的引用，用于存储资源限制
 *
 * 此函数定义了各种图形资源的最大限制，这些限制在图形管线的不同阶段中被使用
 * 包括顶点着色器、片段着色器、几何着色器、曲面细分着色器等
 * 这些限制确保了应用程序在使用这些资源时不会超出硬件或驱动程序的能力范围
 */
void init_resources(TBuiltInResource& Resources)
{
    // 定义最大灯光数量
    Resources.maxLights = 32;
    // 定义最大裁剪平面数量
    Resources.maxClipPlanes = 6;
    // 定义最大纹理单元数量
    Resources.maxTextureUnits = 32;
    // 定义最大纹理坐标数量
    Resources.maxTextureCoords = 32;
    // 定义最大顶点属性数量
    Resources.maxVertexAttribs = 64;
    // 定义顶点着色器中最大uniform组件数量
    Resources.maxVertexUniformComponents = 4096;
    // 定义最大varying浮点数数量
    Resources.maxVaryingFloats = 64;
    // 定义顶点纹理图像单元最大数量
    Resources.maxVertexTextureImageUnits = 32;
    // 定义组合纹理图像单元最大数量
    Resources.maxCombinedTextureImageUnits = 80;
    // 定义纹理图像单元最大数量
    Resources.maxTextureImageUnits = 32;
    // 定义片段着色器中最大uniform组件数量
    Resources.maxFragmentUniformComponents = 4096;
    // 定义最大绘制缓冲数量
    Resources.maxDrawBuffers = 32;
    // 定义顶点着色器中最大uniform向量数量
    Resources.maxVertexUniformVectors = 128;
    // 定义最大varying向量数量
    Resources.maxVaryingVectors = 8;
    // 定义片段着色器中最大uniform向量数量
    Resources.maxFragmentUniformVectors = 16;
    // 定义顶点输出向量最大数量
    Resources.maxVertexOutputVectors = 16;
    // 定义片段输入向量最大数量
    Resources.maxFragmentInputVectors = 15;
    // 定义程序中最小Texel偏移量
    Resources.minProgramTexelOffset = -8;
    // 定义程序中最大Texel偏移量
    Resources.maxProgramTexelOffset = 7;
    // 定义最大裁剪距离数量
    Resources.maxClipDistances = 8;
    // 定义计算工作组最大计数X轴
    Resources.maxComputeWorkGroupCountX = 65535;
    // 定义计算工作组最大计数Y轴
    Resources.maxComputeWorkGroupCountY = 65535;
    // 定义计算工作组最大计数Z轴
    Resources.maxComputeWorkGroupCountZ = 65535;
    // 定义计算工作组最大尺寸X轴
    Resources.maxComputeWorkGroupSizeX = 1024;
    // 定义计算工作组最大尺寸Y轴
    Resources.maxComputeWorkGroupSizeY = 1024;
    // 定义计算工作组最大尺寸Z轴
    Resources.maxComputeWorkGroupSizeZ = 64;
    // 定义计算着色器中最大uniform组件数量
    Resources.maxComputeUniformComponents = 1024;
    // 定义计算纹理图像单元最大数量
    Resources.maxComputeTextureImageUnits = 16;
    // 定义计算图像uniform最大数量
    Resources.maxComputeImageUniforms = 8;
    // 定义计算原子计数最大数量
    Resources.maxComputeAtomicCounters = 8;
    // 定义计算原子计数缓冲最大数量
    Resources.maxComputeAtomicCounterBuffers = 1;
    // 定义最大varying组件数量
    Resources.maxVaryingComponents = 60;
    // 定义顶点输出组件最大数量
    Resources.maxVertexOutputComponents = 64;
    // 定义几何输入组件最大数量
    Resources.maxGeometryInputComponents = 64;
    // 定义几何输出组件最大数量
    Resources.maxGeometryOutputComponents = 128;
    // 定义片段输入组件最大数量
    Resources.maxFragmentInputComponents = 128;
    // 定义图像单元最大数量
    Resources.maxImageUnits = 8;
    // 定义组合图像单元和片段输出最大数量
    Resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
    // 定义组合着色器输出资源最大数量
    Resources.maxCombinedShaderOutputResources = 8;
    // 定义图像采样最大数量
    Resources.maxImageSamples = 0;
    // 定义顶点图像uniform最大数量
    Resources.maxVertexImageUniforms = 0;
    // 定义曲面细分控制图像uniform最大数量
    Resources.maxTessControlImageUniforms = 0;
    // 定义曲面细分评估图像uniform最大数量
    Resources.maxTessEvaluationImageUniforms = 0;
    // 定义几何图像uniform最大数量
    Resources.maxGeometryImageUniforms = 0;
    // 定义片段图像uniform最大数量
    Resources.maxFragmentImageUniforms = 8;
    // 定义组合图像uniform最大数量
    Resources.maxCombinedImageUniforms = 8;
    // 定义几何纹理图像单元最大数量
    Resources.maxGeometryTextureImageUnits = 16;
    // 定义几何输出顶点最大数量
    Resources.maxGeometryOutputVertices = 256;
    // 定义几何总输出组件最大数量
    Resources.maxGeometryTotalOutputComponents = 1024;
    // 定义几何着色器中最大uniform组件数量
    Resources.maxGeometryUniformComponents = 1024;
    // 定义几何varying组件最大数量
    Resources.maxGeometryVaryingComponents = 64;
    // 定义曲面细分控制输入组件最大数量
    Resources.maxTessControlInputComponents = 128;
    // 定义曲面细分控制输出组件最大数量
    Resources.maxTessControlOutputComponents = 128;
    // 定义曲面细分控制纹理图像单元最大数量
    Resources.maxTessControlTextureImageUnits = 16;
    // 定义曲面细分控制着色器中最大uniform组件数量
    Resources.maxTessControlUniformComponents = 1024;
    // 定义曲面细分控制总输出组件最大数量
    Resources.maxTessControlTotalOutputComponents = 4096;
    // 定义曲面细分评估输入组件最大数量
    Resources.maxTessEvaluationInputComponents = 128;
    // 定义曲面细分评估输出组件最大数量
    Resources.maxTessEvaluationOutputComponents = 128;
    // 定义曲面细分评估纹理图像单元最大数量
    Resources.maxTessEvaluationTextureImageUnits = 16;
    // 定义曲面细分评估着色器中最大uniform组件数量
    Resources.maxTessEvaluationUniformComponents = 1024;
    // 定义曲面细分patch组件最大数量
    Resources.maxTessPatchComponents = 120;
    // 定义最大patch顶点数量
    Resources.maxPatchVertices = 32;
    // 定义曲面细分生成级别最大值
    Resources.maxTessGenLevel = 64;
    // 定义最大视口数量
    Resources.maxViewports = 16;
    // 定义顶点原子计数最大数量
    Resources.maxVertexAtomicCounters = 0;
    // 定义曲面细分控制原子计数最大数量
    Resources.maxTessControlAtomicCounters = 0;
    // 定义曲面细分评估原子计数最大数量
    Resources.maxTessEvaluationAtomicCounters = 0;
    // 定义几何原子计数最大数量
    Resources.maxGeometryAtomicCounters = 0;
    // 定义片段原子计数最大数量
    Resources.maxFragmentAtomicCounters = 8;
    // 定义组合原子计数最大数量
    Resources.maxCombinedAtomicCounters = 8;
    // 定义原子计数绑定最大数量
    Resources.maxAtomicCounterBindings = 1;
    // 定义顶点原子计数缓冲最大数量
    Resources.maxVertexAtomicCounterBuffers = 0;
    // 定义曲面细分控制原子计数缓冲最大数量
    Resources.maxTessControlAtomicCounterBuffers = 0;
    // 定义曲面细分评估原子计数缓冲最大数量
    Resources.maxTessEvaluationAtomicCounterBuffers = 0;
    // 定义几何原子计数缓冲最大数量
    Resources.maxGeometryAtomicCounterBuffers = 0;
    // 定义片段原子计数缓冲最大数量
    Resources.maxFragmentAtomicCounterBuffers = 1;
    // 定义组合原子计数缓冲最大数量
    Resources.maxCombinedAtomicCounterBuffers = 1;
    // 定义原子计数缓冲区最大尺寸
    Resources.maxAtomicCounterBufferSize = 16384;
    // 定义最大变换反馈缓冲数量
    Resources.maxTransformFeedbackBuffers = 4;
    // 定义最大变换反馈交错组件数量
    Resources.maxTransformFeedbackInterleavedComponents = 64;
    // 定义最大剔除距离数量
    Resources.maxCullDistances = 8;
    // 定义最大组合裁剪和剔除距离数量
    Resources.maxCombinedClipAndCullDistances = 8;
    // 定义最大样本数量
    Resources.maxSamples = 4;
    // 定义循环限制，非归纳循环
    Resources.limits.nonInductiveForLoops = 1;
    // 定义循环限制，while循环
    Resources.limits.whileLoops = 1;
    // 定义循环限制，do-while循环
    Resources.limits.doWhileLoops = 1;
    // 定义uniform索引限制，一般uniform索引
    Resources.limits.generalUniformIndexing = 1;
    // 定义attribute矩阵/向量索引限制，一般attribute矩阵/向量索引
    Resources.limits.generalAttributeMatrixVectorIndexing = 1;
    // 定义varying索引限制，一般varying索引
    Resources.limits.generalVaryingIndexing = 1;
    // 定义sampler索引限制，一般sampler索引
    Resources.limits.generalSamplerIndexing = 1;
    // 定义变量索引限制，一般变量索引
    Resources.limits.generalVariableIndexing = 1;
    // 定义constant矩阵/向量索引限制，一般constant矩阵/向量索引
    Resources.limits.generalConstantMatrixVectorIndexing = 1;
}

EShLanguage FindLanguage(const VkShaderStageFlagBits shader_type)
{
    switch (shader_type)
    {
    case VK_SHADER_STAGE_VERTEX_BIT:
        return EShLangVertex;

    case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:
        return EShLangTessControl;

    case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:
        return EShLangTessEvaluation;

    case VK_SHADER_STAGE_GEOMETRY_BIT:
        return EShLangGeometry;

    case VK_SHADER_STAGE_FRAGMENT_BIT:
        return EShLangFragment;

    case VK_SHADER_STAGE_COMPUTE_BIT:
        return EShLangCompute;

    default:
        return EShLangVertex;
    }
}

void wait_seconds(int seconds)
{
    Sleep(seconds * 1000);
}

// 获取毫秒数
timestamp_t get_milliseconds()
{
    LARGE_INTEGER frequency;
    BOOL useQPC = QueryPerformanceFrequency(&frequency);
    if (useQPC)
    {
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        return (1000LL * now.QuadPart) / frequency.QuadPart;
    }
    else
    {
        return GetTickCount64();
    }
}

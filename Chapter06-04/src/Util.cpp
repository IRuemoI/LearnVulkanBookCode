#include "Util.h"

using namespace std;

int preX = 0;//鼠标的横坐标
int preY = 0;//鼠标的纵坐标
bool left_mouse_down = false;
bool isClick = true;
int pressType = 1;
void cursor_pos_callback(GLFWwindow *window, double xpos, double ypos) {
    VulkanDemoApp::currentCursorX = xpos;
    VulkanDemoApp::currentCursorY = ypos;

    // 处理鼠标拖动事件
    if (left_mouse_down) {
        VulkanDemoApp::cursorDeltaX = preX - VulkanDemoApp::currentCursorX;
        VulkanDemoApp::cursorDeltaY = preY - VulkanDemoApp::currentCursorY;
        VulkanDemoApp::yAngle = VulkanDemoApp::yAngle + (float) VulkanDemoApp::cursorDeltaX * 180 / 200;
        VulkanDemoApp::zAngle = VulkanDemoApp::zAngle + (float) VulkanDemoApp::cursorDeltaY * 180 / 200;
        if (abs((int) VulkanDemoApp::cursorDeltaX) > 2 || abs((int) VulkanDemoApp::cursorDeltaY) > 2) {
            isClick = false;
        }
        printf("拖动鼠标的表示的旋转:[Y轴:%f,Z轴:%f]\n", VulkanDemoApp::yAngle, VulkanDemoApp::zAngle);
    }

    //保存本次的鼠标位置作为上次的位置
    preX = (int) xpos;
    preY = (int) ypos;
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        left_mouse_down = true;
        printf("鼠标按下\n");
    } else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        if (left_mouse_down) {
            if (isClick) {//点击标志为 true
                switch (pressType) {//根据不同的pressType值分支
                    case 0:
                        VulkanDemoApp::smallType = 0;
                        VulkanDemoApp::bigType = 1;//第1种组合
                        break;
                    case 1:
                        VulkanDemoApp::smallType = 1;
                        VulkanDemoApp::bigType = 0;//第2种组合
                        break;
                    case 2:
                        VulkanDemoApp::smallType = 0;
                        VulkanDemoApp::bigType = 0;//第3种组合
                        break;
                    case 3:
                        VulkanDemoApp::smallType = 1;
                        VulkanDemoApp::bigType = 1;//第4种组合
                        break;
                }
                printf("当前状态:smallType=%d,bigType=%d\n", VulkanDemoApp::smallType, VulkanDemoApp::bigType);
                pressType = (++pressType % 4);//改变pressType值
            }
        }
        left_mouse_down = false;
        printf("鼠标松开\n");
    }
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    printf("scroll called [%F %F]\n", xoffset, yoffset);
}

// 键盘按键回调函数
void processInputs(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        std::cout << "按下了W键" << std::endl;
}

void init_resources(TBuiltInResource &Resources) {
    Resources.maxLights = 32;
    Resources.maxClipPlanes = 6;
    Resources.maxTextureUnits = 32;
    Resources.maxTextureCoords = 32;
    Resources.maxVertexAttribs = 64;
    Resources.maxVertexUniformComponents = 4096;
    Resources.maxVaryingFloats = 64;
    Resources.maxVertexTextureImageUnits = 32;
    Resources.maxCombinedTextureImageUnits = 80;
    Resources.maxTextureImageUnits = 32;
    Resources.maxFragmentUniformComponents = 4096;
    Resources.maxDrawBuffers = 32;
    Resources.maxVertexUniformVectors = 128;
    Resources.maxVaryingVectors = 8;
    Resources.maxFragmentUniformVectors = 16;
    Resources.maxVertexOutputVectors = 16;
    Resources.maxFragmentInputVectors = 15;
    Resources.minProgramTexelOffset = -8;
    Resources.maxProgramTexelOffset = 7;
    Resources.maxClipDistances = 8;
    Resources.maxComputeWorkGroupCountX = 65535;
    Resources.maxComputeWorkGroupCountY = 65535;
    Resources.maxComputeWorkGroupCountZ = 65535;
    Resources.maxComputeWorkGroupSizeX = 1024;
    Resources.maxComputeWorkGroupSizeY = 1024;
    Resources.maxComputeWorkGroupSizeZ = 64;
    Resources.maxComputeUniformComponents = 1024;
    Resources.maxComputeTextureImageUnits = 16;
    Resources.maxComputeImageUniforms = 8;
    Resources.maxComputeAtomicCounters = 8;
    Resources.maxComputeAtomicCounterBuffers = 1;
    Resources.maxVaryingComponents = 60;
    Resources.maxVertexOutputComponents = 64;
    Resources.maxGeometryInputComponents = 64;
    Resources.maxGeometryOutputComponents = 128;
    Resources.maxFragmentInputComponents = 128;
    Resources.maxImageUnits = 8;
    Resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
    Resources.maxCombinedShaderOutputResources = 8;
    Resources.maxImageSamples = 0;
    Resources.maxVertexImageUniforms = 0;
    Resources.maxTessControlImageUniforms = 0;
    Resources.maxTessEvaluationImageUniforms = 0;
    Resources.maxGeometryImageUniforms = 0;
    Resources.maxFragmentImageUniforms = 8;
    Resources.maxCombinedImageUniforms = 8;
    Resources.maxGeometryTextureImageUnits = 16;
    Resources.maxGeometryOutputVertices = 256;
    Resources.maxGeometryTotalOutputComponents = 1024;
    Resources.maxGeometryUniformComponents = 1024;
    Resources.maxGeometryVaryingComponents = 64;
    Resources.maxTessControlInputComponents = 128;
    Resources.maxTessControlOutputComponents = 128;
    Resources.maxTessControlTextureImageUnits = 16;
    Resources.maxTessControlUniformComponents = 1024;
    Resources.maxTessControlTotalOutputComponents = 4096;
    Resources.maxTessEvaluationInputComponents = 128;
    Resources.maxTessEvaluationOutputComponents = 128;
    Resources.maxTessEvaluationTextureImageUnits = 16;
    Resources.maxTessEvaluationUniformComponents = 1024;
    Resources.maxTessPatchComponents = 120;
    Resources.maxPatchVertices = 32;
    Resources.maxTessGenLevel = 64;
    Resources.maxViewports = 16;
    Resources.maxVertexAtomicCounters = 0;
    Resources.maxTessControlAtomicCounters = 0;
    Resources.maxTessEvaluationAtomicCounters = 0;
    Resources.maxGeometryAtomicCounters = 0;
    Resources.maxFragmentAtomicCounters = 8;
    Resources.maxCombinedAtomicCounters = 8;
    Resources.maxAtomicCounterBindings = 1;
    Resources.maxVertexAtomicCounterBuffers = 0;
    Resources.maxTessControlAtomicCounterBuffers = 0;
    Resources.maxTessEvaluationAtomicCounterBuffers = 0;
    Resources.maxGeometryAtomicCounterBuffers = 0;
    Resources.maxFragmentAtomicCounterBuffers = 1;
    Resources.maxCombinedAtomicCounterBuffers = 1;
    Resources.maxAtomicCounterBufferSize = 16384;
    Resources.maxTransformFeedbackBuffers = 4;
    Resources.maxTransformFeedbackInterleavedComponents = 64;
    Resources.maxCullDistances = 8;
    Resources.maxCombinedClipAndCullDistances = 8;
    Resources.maxSamples = 4;
    Resources.limits.nonInductiveForLoops = 1;
    Resources.limits.whileLoops = 1;
    Resources.limits.doWhileLoops = 1;
    Resources.limits.generalUniformIndexing = 1;
    Resources.limits.generalAttributeMatrixVectorIndexing = 1;
    Resources.limits.generalVaryingIndexing = 1;
    Resources.limits.generalSamplerIndexing = 1;
    Resources.limits.generalVariableIndexing = 1;
    Resources.limits.generalConstantMatrixVectorIndexing = 1;
}

EShLanguage FindLanguage(const VkShaderStageFlagBits shader_type) {
    switch (shader_type) {
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

void wait_seconds(int seconds) {
    Sleep(seconds * 1000);
}

// 获取毫秒数
timestamp_t get_milliseconds() {
    LARGE_INTEGER frequency;
    BOOL useQPC = QueryPerformanceFrequency(&frequency);
    if (useQPC) {
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);
        return (1000LL * now.QuadPart) / frequency.QuadPart;
    } else {
        return GetTickCount64();
    }
}

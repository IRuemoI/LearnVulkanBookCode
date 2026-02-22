#pragma once

#ifdef _WIN32
#include <windows.h>
#elif __APPLE__
#include <mach/mach_time.h>
#else
#include <time.h>
#endif

class FPSUtil
{
public:
    // 用于计算FPS的辅助变量
    static int FPSCount;
    static long long startTime;
    static float currFPS;

    // 用于控制帧速率的辅助变量
    static long long beforeTime;

    // 计算FPS相关方法
    static void init();
    static void calFPS();

    // 控制帧速率相关方法
    static void before();
    static void after(int dstFPS);

    // 跨平台获取毫秒时间函数
    static long long getCurrentTimeMillis();
};

#include "FPSUtil.h"

// 静态变量定义
int FPSUtil::FPSCount = 0;
long long FPSUtil::startTime = 0;
float FPSUtil::currFPS = 0.0f;
long long FPSUtil::beforeTime = 0;

void FPSUtil::init()
{
    FPSCount = 0;
    startTime = getCurrentTimeMillis();
    currFPS = 0.0f;
}

void FPSUtil::calFPS()
{
    FPSCount++;
    long long endTime = getCurrentTimeMillis();
    if (endTime - startTime > 1000)
    {
        currFPS = (float)(FPSCount * 1000.0 / (endTime - startTime));
        startTime = endTime;
        FPSCount = 0;
    }
}

void FPSUtil::before()
{
    beforeTime = getCurrentTimeMillis();
}

void FPSUtil::after(int dstFPS)
{
    long long targetFrameTime = 1000 / dstFPS;
    long long frameTime = getCurrentTimeMillis() - beforeTime;

    if (frameTime < targetFrameTime)
    {
        long long sleepTime = targetFrameTime - frameTime;
#ifdef _WIN32
        Sleep(sleepTime);
#elif __APPLE__
        usleep(sleepTime * 1000);
#else
        usleep(sleepTime * 1000);
#endif
    }
}

long long FPSUtil::getCurrentTimeMillis()
{
#ifdef _WIN32
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
#elif __APPLE__
    static mach_timebase_info_data_t timebase;
    if (timebase.denom == 0)
    {
        mach_timebase_info(&timebase);
    }
    uint64_t time = mach_absolute_time();
    return (time * timebase.numer) / (timebase.denom * 1000000);
#else
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#endif
}

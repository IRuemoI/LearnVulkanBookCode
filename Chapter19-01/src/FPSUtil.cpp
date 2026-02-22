#include "FPSUtil.h"
#include <time.h>
#include <chrono>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

int FPSUtil::FPSCount;
long long FPSUtil::startTime;
float FPSUtil::currFPS;
long long FPSUtil::beforeTime;

long long currentTimeMillis()
{
#ifdef _WIN32
    return GetTickCount();
#else
    struct timeval tv;
    gettimeofday(&tv, nullptr);
    return (long long)(tv.tv_sec * 1000 + tv.tv_usec / 1000);
#endif
}

void FPSUtil::init()
{
    FPSCount = 0;
    startTime = currentTimeMillis();
}

void FPSUtil::calFPS()
{
    FPSCount++;
    if (FPSCount == 100)
    {
        FPSCount = 0;
        long long endTime = currentTimeMillis();
        currFPS = (float)(1000.0 / ((endTime - startTime) / 100.0));
        startTime = endTime;
        printf("FPS: %f\n", FPSUtil::currFPS);
    }
}

void FPSUtil::before()
{
    beforeTime = currentTimeMillis();
}

void FPSUtil::after(int dstFPS)
{
    int dstSpan = (int)(1000 / dstFPS) + 1;
    long long span = currentTimeMillis() - beforeTime;
    if (span < dstSpan)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(dstSpan - span));
    }
}

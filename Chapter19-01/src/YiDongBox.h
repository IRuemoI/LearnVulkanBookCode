#ifndef VULKANEXBASE_YIDONGBOX_H
#define VULKANEXBASE_YIDONGBOX_H
#include <string>
#include "ObjObject.h"
#include "FileUtil.h"
#include "MyVulkanManager.h"
using namespace std;

class YiDongBox
{
public:
    static int yidong_time;
    static bool yidong_up;
    static int YiDongIndex;
    static float YiDongUpDown();
    static void YiDongTimeUpdate();
    static void YiDongCheckLife();
};


#endif //VULKANEXBASE_YIDONGBOX_H

#ifndef VULKANEXBASE_DIAOLUOMANAGER_H
#define VULKANEXBASE_DIAOLUOMANAGER_H

#include <string>
#include "ObjObject.h"
#include "FileUtil.h"
#include "MyVulkanManager.h"
using namespace std;

class DiaoLuoManager
{
public:
    static int diaoluo_time;
    static bool diaoluo_up;
    static bool diaoluo_down;
    static int DiaoLuoIndex;
    static float DiaoLuoUpDown();
    static void DiaoLuoTimeUpdate();
    static void DiaoLuoCheckLife();
};


#endif //VULKANEXBASE_DIAOLUOMANAGER_H

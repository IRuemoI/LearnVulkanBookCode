#ifndef DiCiBox_H
#define DiCiBox_H

#include <string>
#include "ObjObject.h"
#include "FileUtil.h"
#include "MyVulkanManager.h"
using namespace std;

class DiCiBox
{
public:
    static int dici_time;
    static bool dici_up;
    static int DiCiIndex;
    static float DiCiUpDown();
    static void DiCiTimeUpdate();
    static void DiCiCheckLife();
};

#endif

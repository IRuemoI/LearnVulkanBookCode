//
// Created by song on 2017/8/3.
//

#ifndef VULKANEXBASE_SQUARE_JIAZIBOX_H
#define VULKANEXBASE_SQUARE_JIAZIBOX_H


#include <string>
#include "ObjObject.h"
#include "FileUtil.h"
#include "MyVulkanManager.h"
using namespace std;

class JiaZiBox
{
public:
    static int jiazi_time;
    static bool jiazi_up;
    static int JiaZiIndex;
    static void JiaZiTimeUpdate();
    static float JiaZiUpDown();
    static void JiaZiCheckLife();
};

#endif //VULKANEXBASE_SQUARE_JIAZIBOX_H

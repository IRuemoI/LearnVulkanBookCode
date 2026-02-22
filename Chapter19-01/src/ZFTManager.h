#ifndef VULKANEXBASE_ZFTMANAGER_H
#define VULKANEXBASE_ZFTMANAGER_H

#include "Square3D.h"

class ZFTManager
{
public:
    static bool life;
    static int Diefoncution;
    static int Box_pos;
    static float MainScalex;
    static float MainScaley;
    static float MainScalez;
    static void Update_mainbox();
    static void ZFTDiCiDie();
    static void ZFTJiaZiDie();
    static void ZFTDiaoLuoDie();
    static void ZFTYiDongDie();
    static void Win();
};

#endif //VULKANEXBASE_ZFTMANAGER_H

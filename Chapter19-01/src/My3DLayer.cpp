#include "My3DLayer.h"
#include "ZFTManager.h"
#include "MyDraw.h"
#include "DiaoLuoManager.h"
#include "DiCiBox.h"
#include "JiaZiBox.h"
#include "TanShe.h"
#include "YiDongBox.h"
#include "Touch2D.h"
int My3DLayer::flag[TREE_NUMBER] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 2, 1, 2, 1, 2, 1, 2, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
int My3DLayer::flag1[TREE_NUMBER] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 2, 1, 2, 1, 2, 1, 2, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
int My3DLayer::flag2[TREE_NUMBER] = {
    1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};
int My3DLayer::flagGo[TREE_NUMBER] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 1, 2, 1, 2, 1, 2, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
int My3DLayer::flag1Go[TREE_NUMBER] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    2, 1, 2, 1, 2, 1, 2, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
int My3DLayer::flag2Go[TREE_NUMBER] = {
    1, 1, 1, 1, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};
int My3DLayer::dicipos[DICI_NUMBER] = {4, 13, 19, 20, 21, 30, 42};
int My3DLayer::jiazipos[JIAZI_NUMBER] = {7, 8, 11, 25, 48};
int My3DLayer::diaoluopos[DIAOLUO_NUMBER] = {15, 27, 36, 40};
int My3DLayer::yidongpos[YIDONG_NUMBER] = {3};
int My3DLayer::tanshepos[TANSHE_NUMBER] = {5};

int My3DLayer::dicipos1[DICI_NUMBER] = {4, 13, 19, 20, 21, 30, 42};
int My3DLayer::jiazipos1[JIAZI_NUMBER] = {7, 8, 11, 25, 48};
int My3DLayer::diaoluopos1[DIAOLUO_NUMBER] = {15, 27, 36, 40};
int My3DLayer::yidongpos1[YIDONG_NUMBER] = {3};
int My3DLayer::tanshepos1[TANSHE_NUMBER] = {5};

int My3DLayer::dicipos2[DICI_NUMBER] = {4, 13, 19, 20, 21, 30, 39};
int My3DLayer::jiazipos2[JIAZI_NUMBER] = {7, 8, 11, 25, 48};
int My3DLayer::diaoluopos2[DIAOLUO_NUMBER] = {15, 27, 36, 41};
int My3DLayer::yidongpos2[YIDONG_NUMBER] = {3};
int My3DLayer::tanshepos2[TANSHE_NUMBER] = {5};
int My3DLayer::attachbox_draw_count[ATTACH_BOX_NUMBER] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

int My3DLayer::BoxPos = 0;
bool My3DLayer::BoxPos_licence = false;
bool My3DLayer::attachbox_draw_licence[ATTACH_BOX_NUMBER] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
int My3DLayer::Level = 1;

void My3DLayer::initLevel()
{
    if (My3DLayer::Level > 2)
    {
        My3DLayer::Level = 1;
    }
    if (My3DLayer::Level == 1)
    {
        for (int i = 0; i < TREE_NUMBER; i++)
        {
            My3DLayer::flag[i] = My3DLayer::flag1[i];
            My3DLayer::flagGo[i] = My3DLayer::flag1Go[i];
        }
        for (int i = 0; i < DICI_NUMBER; i++)
        {
            My3DLayer::dicipos[i] = My3DLayer::dicipos1[i];
        }
        for (int i = 0; i < JIAZI_NUMBER; i++)
        {
            My3DLayer::jiazipos[i] = My3DLayer::jiazipos1[i];
        }
        for (int i = 0; i < DIAOLUO_NUMBER; i++)
        {
            My3DLayer::diaoluopos[i] = My3DLayer::diaoluopos1[i];
        }
        for (int i = 0; i < YIDONG_NUMBER; i++)
        {
            My3DLayer::yidongpos[i] = My3DLayer::yidongpos1[i];
        }
        for (int i = 0; i < TANSHE_NUMBER; i++)
        {
            My3DLayer::tanshepos[i] = My3DLayer::tanshepos1[i];
        }
    }
    else if (My3DLayer::Level == 2)
    {
        for (int i = 0; i < TREE_NUMBER; i++)
        {
            My3DLayer::flag[i] = My3DLayer::flag2[i];
            My3DLayer::flagGo[i] = My3DLayer::flag2Go[i];
        }
        for (int i = 0; i < DICI_NUMBER; i++)
        {
            My3DLayer::dicipos[i] = My3DLayer::dicipos2[i];
        }
        for (int i = 0; i < JIAZI_NUMBER; i++)
        {
            My3DLayer::jiazipos[i] = My3DLayer::jiazipos2[i];
        }
        for (int i = 0; i < DIAOLUO_NUMBER; i++)
        {
            My3DLayer::diaoluopos[i] = My3DLayer::diaoluopos2[i];
        }
        for (int i = 0; i < YIDONG_NUMBER; i++)
        {
            My3DLayer::yidongpos[i] = My3DLayer::yidongpos2[i];
        }
        for (int i = 0; i < TANSHE_NUMBER; i++)
        {
            My3DLayer::tanshepos[i] = My3DLayer::tanshepos2[i];
        }
    }
}

void My3DLayer::NextLevel()
{
    for (int i = 1; i < ATTACH_BOX_NUMBER; i++)
    {
        My3DLayer::attachbox_draw_licence[i] = 0;
        My3DLayer::attachbox_draw_count[i] = 0;
    }
    My3DLayer::BoxPos_licence = false;
    My3DLayer::mainBox->setPosition3D(0, 100, 0);
    My3DLayer::mainBox->Ry = 0;
    My3DLayer::mainBox->Rx = 0;
    My3DLayer::mainBox->Rz = 0;
    My3DLayer::mainBox->Rangle = 0;
    My3DLayer::Level += 1;
    DiaoLuoManager::DiaoLuoIndex = 0;
    DiCiBox::DiCiIndex = 0;
    JiaZiBox::JiaZiIndex = 0;
    YiDongBox::YiDongIndex = 0;
    My3DLayer::initLevel();
    MyDraw::InitDrawobject();
    My3DLayer::BoxPos = 0;
    ZFTManager::life = true;
    ZFTManager::Box_pos = 0;
    Touch2D::Button_start = false;
}

void My3DLayer::RepeatLevel()
{
    for (int i = 1; i < ATTACH_BOX_NUMBER; i++)
    {
        My3DLayer::attachbox_draw_licence[i] = 0;
        My3DLayer::attachbox_draw_count[i] = 0;
    }
    My3DLayer::BoxPos_licence = false;
    My3DLayer::mainBox->setPosition3D(0, 100, 0);
    ZFTManager::MainScalex = 1;
    ZFTManager::MainScaley = 1;
    ZFTManager::MainScalez = 1;
    DiCiBox::DiCiIndex = 0;
    JiaZiBox::JiaZiIndex = 0;
    YiDongBox::YiDongIndex = 0;
    My3DLayer::BoxPos = 0;
    ZFTManager::life = true;
    ZFTManager::Box_pos = 0;
    Touch2D::Button_resume = false;
}

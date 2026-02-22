#ifndef VULKANEXBASE_MY3DLAYER_H
#define VULKANEXBASE_MY3DLAYER_H

#include "ObjObject.h"
#define ATTACH_BOX_NUMBER 50//¸½Êô·½¿é¸öÊý
#define TREE_NUMBER 50	//µØÍ¼¿éÊý
#define DICI_NUMBER 7	//µØ´Ì¸öÊý
#define JIAZI_NUMBER 5	//¼Ð×Ó¸öÊý
#define TANSHE_NUMBER 1 //µ¯Éä¸öÊý
#define DIAOLUO_NUMBER 4//µôÂä¸öÊý
#define YIDONG_NUMBER 1//ÒÆ¶¯¸öÊý
#define QIZIGUN_NUMBER 2//Æì×Ó¹÷
#define Goorientationright 0
#define Goorientationstright 1
#define Goorientationleft 2
#define Goorientationback 3
#define Goorientationinit 4

class My3DLayer
{
public:
    static int flag[TREE_NUMBER];
    static int flagGo[TREE_NUMBER];
    static int flag1[TREE_NUMBER];
    static int flag1Go[TREE_NUMBER];
    static int flag2[TREE_NUMBER];
    static int flag2Go[TREE_NUMBER];
    static int dicipos[DICI_NUMBER];
    static int tanshepos[TANSHE_NUMBER];
    static int jiazipos[JIAZI_NUMBER];
    static int diaoluopos[DIAOLUO_NUMBER];
    static int yidongpos[YIDONG_NUMBER];
    static int tanshepos1[TANSHE_NUMBER];
    static int dicipos1[DICI_NUMBER];
    static int jiazipos1[JIAZI_NUMBER];
    static int diaoluopos1[DIAOLUO_NUMBER];
    static int yidongpos1[YIDONG_NUMBER];
    static int dicipos2[DICI_NUMBER];
    static int jiazipos2[JIAZI_NUMBER];
    static int diaoluopos2[DIAOLUO_NUMBER];
    static int yidongpos2[YIDONG_NUMBER];
    static int tanshepos2[TANSHE_NUMBER];

    static int attachbox_draw_count[ATTACH_BOX_NUMBER];
    static int BoxPos;
    static bool BoxPos_licence;
    static bool attachbox_draw_licence[ATTACH_BOX_NUMBER];
    static int Level;
    static ObjObject* sp3Tree[TREE_NUMBER];
    static ObjObject* sp3Dici[DICI_NUMBER];
    static ObjObject* sp3JiaziLeft[JIAZI_NUMBER];
    static ObjObject* sp3JiaziRight[JIAZI_NUMBER];
    static ObjObject* sp3JiaziLeftClose[JIAZI_NUMBER];
    static ObjObject* sp3JiaziRightClose[JIAZI_NUMBER];
    static ObjObject* sp3DiaoLuo[DIAOLUO_NUMBER];
    static ObjObject* sp3DiaoLuoshadow[DIAOLUO_NUMBER];
    static ObjObject* sp3YiDong[YIDONG_NUMBER];
    static ObjObject* planeForDraw;
    static ObjObject* mainBox;
    static ObjObject* attachBox[ATTACH_BOX_NUMBER];
    static ObjObject* attachBox2[ATTACH_BOX_NUMBER];
    static ObjObject* attachBox3[ATTACH_BOX_NUMBER];
    static ObjObject* attachBox4[ATTACH_BOX_NUMBER];
    static ObjObject* attachBox5[ATTACH_BOX_NUMBER];
    static ObjObject* attachBox6[ATTACH_BOX_NUMBER];
    static ObjObject* attachBox7[ATTACH_BOX_NUMBER];
    static ObjObject* attachBox8[ATTACH_BOX_NUMBER];
    static ObjObject* tanshe[TANSHE_NUMBER];
    static ObjObject* qizi;
    static ObjObject* qizigun[QIZIGUN_NUMBER];
    static void initLevel();
    static void NextLevel();
    static void RepeatLevel();
};
#endif //VULKANEXBASE_MY3DLAYER_H

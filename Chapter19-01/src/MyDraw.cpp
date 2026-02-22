#include "MyDraw.h"
#include "MyVulkanManager.h"
#include "My3DLayer.h"
#include "Square3D.h"
#include "DiCiBox.h"
#include "JiaZiBox.h"
#include "YiDongBox.h"
#include "DiaoLuoManager.h"
#include "TanShe.h"
#include "MatrixState3D.h"
#include "MatrixState2D.h"
#include "ZFTManager.h"
#include "Touch2D.h"
#include "CameraUtil.h"
#include "PathData.h"


void MyDraw::CreateDrawobject()
{
    My3DLayer::planeForDraw = Square3D::create(plane_obj, floor_texture);

    My3DLayer::sp3Tree[0] = Square3D::create(cft_obj, cft_texture);
    for (int i = 1; i < TREE_NUMBER; i++)
    {
        My3DLayer::sp3Tree[i] = Square3D::create(cft_obj, cft_texture);
    }
    for (int i = 0; i < DICI_NUMBER; i++)
    {
        My3DLayer::sp3Dici[i] = Square3D::create(dici_obj, dici_texture);
    }
    for (int i = 0; i < JIAZI_NUMBER; i++)
    {
        My3DLayer::sp3JiaziLeft[i] = Square3D::create(jiazike_obj, jiazi_texture);
        My3DLayer::sp3JiaziRight[i] = Square3D::create(jiazike_obj, jiazi_texture);
        My3DLayer::sp3JiaziLeftClose[i] = Square3D::create(jiazijia_obj, dici_texture);
        My3DLayer::sp3JiaziRightClose[i] = Square3D::create(jiazijia_obj, dici_texture);
    }
    for (int i = 0; i < DIAOLUO_NUMBER; i++)
    {
        //µØÂä
        My3DLayer::sp3DiaoLuo[i] = Square3D::create(diaoluo_obj, cft_texture);
        My3DLayer::sp3DiaoLuoshadow[i] = Square3D::create(zft_obj, cftshadow_texture);
    }
    for (int i = 0; i < YIDONG_NUMBER; i++)
    {
        My3DLayer::sp3YiDong[i] = Square3D::create(cft_obj, cft_texture);
    }
    My3DLayer::mainBox = Square3D::create(zft_obj, zft_texture);
    for (int i = 0; i < ATTACH_BOX_NUMBER; i++)
    {
        My3DLayer::attachBox[i] = Square3D::create(attach_obj, zft_texture);
        My3DLayer::attachBox2[i] = Square3D::create(attach_obj, zft_texture);
        My3DLayer::attachBox3[i] = Square3D::create(attach_obj, zft_texture);
        My3DLayer::attachBox4[i] = Square3D::create(attach_obj, zft_texture);
        My3DLayer::attachBox5[i] = Square3D::create(attach_obj, zft_texture);
        My3DLayer::attachBox6[i] = Square3D::create(attach_obj, zft_texture);
        My3DLayer::attachBox7[i] = Square3D::create(attach_obj, zft_texture);
        My3DLayer::attachBox8[i] = Square3D::create(attach_obj, zft_texture);
    }

    My3DLayer::qizi = Square3D::create(qizi_obj, qizi_texture);
    for (int i = 0; i < QIZIGUN_NUMBER; i++)
    {
        My3DLayer::qizigun[i] = Square3D::create(qizigun_obj, qizigun_texture);
    }

    for (int i = 0; i < TANSHE_NUMBER; i++)
    {
        My3DLayer::tanshe[i] = Square3D::create(tanshe_obj, tanshe_texture);
    }
}

void MyDraw::InitDrawobject()
{
    My3DLayer::sp3Tree[0]->setPosition3D(0, 40, 0);
    for (int i = 1; i < TREE_NUMBER; i++)
    {
        switch (My3DLayer::flag[i])
        {
        case 0:
            My3DLayer::sp3Tree[i]->setPosition3D(My3DLayer::sp3Tree[i - 1]->x - 100, 40,
                                                 My3DLayer::sp3Tree[i - 1]->z);
            break;
        case 1:
            My3DLayer::sp3Tree[i]->setPosition3D(My3DLayer::sp3Tree[i - 1]->x, 40,
                                                 My3DLayer::sp3Tree[i - 1]->z + 100);
            break;
        case 2:
            My3DLayer::sp3Tree[i]->setPosition3D(My3DLayer::sp3Tree[i - 1]->x + 100, 40,
                                                 My3DLayer::sp3Tree[i - 1]->z);
            break;
        }
    }
    for (int i = 0; i < DICI_NUMBER; i++)
    {
        My3DLayer::sp3Dici[i]->setPosition3D(My3DLayer::sp3Tree[My3DLayer::dicipos[i]]->x, 38,
                                             My3DLayer::sp3Tree[My3DLayer::dicipos[i]]->z);
    }
    for (int i = 0; i < JIAZI_NUMBER; i++)
    {
        My3DLayer::sp3JiaziLeft[i]->setPosition3D(My3DLayer::sp3Tree[My3DLayer::jiazipos[i]]->x, 30,
                                                  My3DLayer::sp3Tree[My3DLayer::jiazipos[i]]->z);
        My3DLayer::sp3JiaziRight[i]->setPosition3D(My3DLayer::sp3Tree[My3DLayer::jiazipos[i]]->x, 30,
                                                   My3DLayer::sp3Tree[My3DLayer::jiazipos[i]]->z);

        My3DLayer::sp3JiaziLeftClose[i]->setPosition3D(My3DLayer::sp3Tree[My3DLayer::jiazipos[i]]->x, 90,
                                                       My3DLayer::sp3Tree[My3DLayer::jiazipos[i]]->z);
        My3DLayer::sp3JiaziRightClose[i]->setPosition3D(My3DLayer::sp3Tree[My3DLayer::jiazipos[i]]->x, 90,
                                                        My3DLayer::sp3Tree[My3DLayer::jiazipos[i]]->z);
    }
    for (int i = 0; i < DIAOLUO_NUMBER; i++)
    {
        My3DLayer::sp3DiaoLuo[i]->setPosition3D(My3DLayer::sp3Tree[My3DLayer::diaoluopos[i]]->x, 80,
                                                My3DLayer::sp3Tree[My3DLayer::diaoluopos[i]]->z);
        My3DLayer::sp3DiaoLuoshadow[i]->setPosition3D(My3DLayer::sp3Tree[My3DLayer::diaoluopos[i]]->x, 65,
                                                      My3DLayer::sp3Tree[My3DLayer::diaoluopos[i]]->z);
    }
    for (int i = 0; i < YIDONG_NUMBER; i++)
    {
        My3DLayer::sp3YiDong[i]->setPosition3D(My3DLayer::sp3Tree[My3DLayer::yidongpos[i]]->x, 60,
                                               My3DLayer::sp3Tree[My3DLayer::yidongpos[i]]->z);
    }
    My3DLayer::mainBox->setPosition3D(0, 100, 0);
    for (int i = 0; i < ATTACH_BOX_NUMBER; i++)
    {
        My3DLayer::attachBox[i]->setPosition3D(My3DLayer::sp3Tree[i]->x, 70, My3DLayer::sp3Tree[i]->z);
        My3DLayer::attachBox2[i]->setPosition3D(My3DLayer::sp3Tree[i]->x, 70, My3DLayer::sp3Tree[i]->z);
        My3DLayer::attachBox3[i]->setPosition3D(My3DLayer::sp3Tree[i]->x, 70, My3DLayer::sp3Tree[i]->z);
        My3DLayer::attachBox4[i]->setPosition3D(My3DLayer::sp3Tree[i]->x, 70, My3DLayer::sp3Tree[i]->z);
        My3DLayer::attachBox5[i]->setPosition3D(My3DLayer::sp3Tree[i]->x, 70, My3DLayer::sp3Tree[i]->z);
        My3DLayer::attachBox6[i]->setPosition3D(My3DLayer::sp3Tree[i]->x, 70, My3DLayer::sp3Tree[i]->z);
        My3DLayer::attachBox7[i]->setPosition3D(My3DLayer::sp3Tree[i]->x, 70, My3DLayer::sp3Tree[i]->z);
        My3DLayer::attachBox8[i]->setPosition3D(My3DLayer::sp3Tree[i]->x, 70, My3DLayer::sp3Tree[i]->z);
    }

    My3DLayer::qizi->setPosition3D(0, 120, 0);
    for (int i = 0; i < QIZIGUN_NUMBER; i++)
    {
        My3DLayer::qizigun[i]->setPosition3D(0, 120, 0);
    }

    for (int i = 0; i < TANSHE_NUMBER; i++)
    {
        My3DLayer::tanshe[i]->setPosition3D(My3DLayer::sp3Tree[My3DLayer::tanshepos[i]]->x, 65,
                                            My3DLayer::sp3Tree[My3DLayer::tanshepos[i]]->z);
    }
}

void MyDraw::Draw3Dobject()
{
    DiCiBox::DiCiTimeUpdate();
    JiaZiBox::JiaZiTimeUpdate();
    DiaoLuoManager::DiaoLuoTimeUpdate();
    YiDongBox::YiDongTimeUpdate();
    MatrixState3D::pushMatrix();
    MatrixState3D::translate(0, 0, 1500);
    MatrixState3D::scale(6, 6, 6);
    My3DLayer::planeForDraw->drawSelf(MyVulkanManager::cmdBuffer, MyVulkanManager::sqsCTL->pipelineLayout,
                                      MyVulkanManager::sqsCTL->pipeline,
                                      &(MyVulkanManager::sqsCTL->descSet[
                                          TextureManager::getVkDescriptorSetIndexForCommonTexLight(
                                              My3DLayer::planeForDraw->texturename)]));
    MatrixState3D::popMatrix();

    for (int i = 0; i < TREE_NUMBER; i++)
    {
        for (int j = 0; j < YIDONG_NUMBER; j++)
        {
            if (i != My3DLayer::yidongpos[j])
            {
                MatrixState3D::pushMatrix();
                MatrixState3D::translate(My3DLayer::sp3Tree[i]->x, My3DLayer::sp3Tree[i]->y, My3DLayer::sp3Tree[i]->z);
                My3DLayer::sp3Tree[i]->drawSelf(MyVulkanManager::cmdBuffer, MyVulkanManager::sqsCTL->pipelineLayout,
                                                MyVulkanManager::sqsCTL->pipeline,
                                                &(MyVulkanManager::sqsCTL->descSet[
                                                    TextureManager::getVkDescriptorSetIndexForCommonTexLight(
                                                        My3DLayer::sp3Tree[i]->texturename)]));
                MatrixState3D::popMatrix();
            }
        }
    }
    for (int i = 0; i < DICI_NUMBER; i++)
    {
        MatrixState3D::pushMatrix();
        MatrixState3D::translate(My3DLayer::sp3Dici[i]->x, My3DLayer::sp3Dici[i]->y + DiCiBox::DiCiUpDown(),
                                 My3DLayer::sp3Dici[i]->z);
        My3DLayer::sp3Dici[i]->drawSelf(MyVulkanManager::cmdBuffer, MyVulkanManager::sqsCTL->pipelineLayout,
                                        MyVulkanManager::sqsCTL->pipeline,
                                        &(MyVulkanManager::sqsCTL->descSet[
                                            TextureManager::getVkDescriptorSetIndexForCommonTexLight(
                                                My3DLayer::sp3Dici[i]->texturename)]));
        MatrixState3D::popMatrix();
    }
    for (int i = 0; i < JIAZI_NUMBER; i++)
    {
        MatrixState3D::pushMatrix();
        if (My3DLayer::flag[My3DLayer::jiazipos[i]] != 1)
        {
            MatrixState3D::translate(My3DLayer::sp3JiaziLeft[i]->x, My3DLayer::sp3JiaziLeft[i]->y,
                                     My3DLayer::sp3JiaziLeft[i]->z - JiaZiBox::JiaZiUpDown());
            MatrixState3D::rotate(90.0, 0.0, 1.0, 0.0);
        }
        else
        {
            MatrixState3D::translate(My3DLayer::sp3JiaziLeft[i]->x - JiaZiBox::JiaZiUpDown(),
                                     My3DLayer::sp3JiaziLeft[i]->y, My3DLayer::sp3JiaziLeft[i]->z);
        }
        My3DLayer::sp3JiaziLeft[i]->drawSelf(MyVulkanManager::cmdBuffer, MyVulkanManager::sqsCTL->pipelineLayout,
                                             MyVulkanManager::sqsCTL->pipeline,
                                             &(MyVulkanManager::sqsCTL->descSet[
                                                 TextureManager::getVkDescriptorSetIndexForCommonTexLight(
                                                     My3DLayer::sp3JiaziLeft[i]->texturename)]));
        MatrixState3D::popMatrix();

        MatrixState3D::pushMatrix();
        if (My3DLayer::flag[My3DLayer::jiazipos[i]] != 1)
        {
            MatrixState3D::translate(My3DLayer::sp3JiaziRight[i]->x, My3DLayer::sp3JiaziRight[i]->y,
                                     My3DLayer::sp3JiaziRight[i]->z + JiaZiBox::JiaZiUpDown());
            MatrixState3D::rotate(90.0, 0.0, 1.0, 0.0);
        }
        else
        {
            MatrixState3D::translate(My3DLayer::sp3JiaziRight[i]->x + JiaZiBox::JiaZiUpDown(),
                                     My3DLayer::sp3JiaziRight[i]->y, My3DLayer::sp3JiaziRight[i]->z);
        }
        My3DLayer::sp3JiaziRight[i]->drawSelf(MyVulkanManager::cmdBuffer, MyVulkanManager::sqsCTL->pipelineLayout,
                                              MyVulkanManager::sqsCTL->pipeline,
                                              &(MyVulkanManager::sqsCTL->descSet[
                                                  TextureManager::getVkDescriptorSetIndexForCommonTexLight(
                                                      My3DLayer::sp3JiaziRight[i]->texturename)]));
        MatrixState3D::popMatrix();


        MatrixState3D::pushMatrix();
        if (My3DLayer::flag[My3DLayer::jiazipos[i]] != 1)
        {
            MatrixState3D::translate(My3DLayer::sp3JiaziLeftClose[i]->x, My3DLayer::sp3JiaziLeftClose[i]->y,
                                     My3DLayer::sp3JiaziLeftClose[i]->z - JiaZiBox::JiaZiUpDown());
            MatrixState3D::rotate(90, 1, 0, 0);
            MatrixState3D::rotate(90 - JiaZiBox::JiaZiUpDown() * 5, 0, 1, 0);
        }
        else
        {
            MatrixState3D::translate(My3DLayer::sp3JiaziLeftClose[i]->x - JiaZiBox::JiaZiUpDown(),
                                     My3DLayer::sp3JiaziLeftClose[i]->y, My3DLayer::sp3JiaziLeftClose[i]->z);
            MatrixState3D::rotate(90, 0, 0, 1);
            MatrixState3D::rotate(90 - JiaZiBox::JiaZiUpDown() * 5, 0, 1, 0);
        }

        MatrixState3D::scale(0.4f, 0.4f, 0.4f);
        My3DLayer::sp3JiaziLeftClose[i]->drawSelf(MyVulkanManager::cmdBuffer, MyVulkanManager::sqsCTL->pipelineLayout,
                                                  MyVulkanManager::sqsCTL->pipeline,
                                                  &(MyVulkanManager::sqsCTL->descSet[
                                                      TextureManager::getVkDescriptorSetIndexForCommonTexLight(
                                                          My3DLayer::sp3JiaziLeftClose[i]->texturename)]));
        MatrixState3D::popMatrix();
    }

    //»æÖÆµôÂä
    for (int i = 0; i < DIAOLUO_NUMBER; i++)
    {
        MatrixState3D::pushMatrix();
        MatrixState3D::translate(My3DLayer::sp3DiaoLuoshadow[i]->x, My3DLayer::sp3DiaoLuoshadow[i]->y,
                                 My3DLayer::sp3DiaoLuoshadow[i]->z);
        MatrixState3D::scale(1.2f - DiaoLuoManager::DiaoLuoUpDown() * 0.01f, 0.02f,
                             1.2f - DiaoLuoManager::DiaoLuoUpDown() * 0.01f);
        My3DLayer::sp3DiaoLuoshadow[i]->drawSelf(MyVulkanManager::cmdBuffer, MyVulkanManager::sqsCTL->pipelineLayout,
                                                 MyVulkanManager::sqsCTL->pipeline,
                                                 &(MyVulkanManager::sqsCTL->descSet[
                                                     TextureManager::getVkDescriptorSetIndexForCommonTexLight(
                                                         My3DLayer::sp3DiaoLuoshadow[i]->texturename)]));
        MatrixState3D::popMatrix();
        MatrixState3D::pushMatrix();
        MatrixState3D::translate(My3DLayer::sp3DiaoLuo[i]->x,
                                 My3DLayer::sp3DiaoLuo[i]->y + DiaoLuoManager::DiaoLuoUpDown(),
                                 My3DLayer::sp3DiaoLuo[i]->z);
        My3DLayer::sp3DiaoLuo[i]->drawSelf(MyVulkanManager::cmdBuffer, MyVulkanManager::sqsCTL->pipelineLayout,
                                           MyVulkanManager::sqsCTL->pipeline,
                                           &(MyVulkanManager::sqsCTL->descSet[
                                               TextureManager::getVkDescriptorSetIndexForCommonTexLight(
                                                   My3DLayer::sp3DiaoLuo[i]->texturename)]));
        MatrixState3D::popMatrix();
    }

    for (int i = 0; i < YIDONG_NUMBER; i++)
    {
        MatrixState3D::pushMatrix();
        if (My3DLayer::flag[My3DLayer::yidongpos[i]] != 1)
        {
            MatrixState3D::translate(My3DLayer::sp3YiDong[i]->x, My3DLayer::sp3YiDong[i]->y,
                                     My3DLayer::sp3YiDong[i]->z + YiDongBox::YiDongUpDown());
            MatrixState3D::rotate(90.0, 0.0, 1.0, 0.0);
        }
        else
        {
            MatrixState3D::translate(My3DLayer::sp3YiDong[i]->x + YiDongBox::YiDongUpDown(), My3DLayer::sp3YiDong[i]->y,
                                     My3DLayer::sp3YiDong[i]->z);
        }
        MatrixState3D::scale(1.0f, 0.05f, 1.0f);
        My3DLayer::sp3YiDong[i]->drawSelf(MyVulkanManager::cmdBuffer, MyVulkanManager::sqsCTL->pipelineLayout,
                                          MyVulkanManager::sqsCTL->pipeline,
                                          &(MyVulkanManager::sqsCTL->descSet[
                                              TextureManager::getVkDescriptorSetIndexForCommonTexLight(
                                                  My3DLayer::sp3YiDong[i]->texturename)]));
        MatrixState3D::popMatrix();
    }

    MatrixState3D::pushMatrix();
    if (My3DLayer::flag[2] != 1)
    {
        MatrixState3D::translate(My3DLayer::sp3Tree[2]->x, My3DLayer::qizi->y, My3DLayer::sp3Tree[2]->z);
    }
    else
    {
        MatrixState3D::translate(My3DLayer::sp3Tree[2]->x, My3DLayer::qizi->y, My3DLayer::sp3Tree[2]->z);
        MatrixState3D::rotate(90.0, 0.0, 1.0, 0.0);
    }
    MatrixState3D::scale(1.1, 1.1, 1.1);
    My3DLayer::qizi->drawSelf(MyVulkanManager::cmdBuffer, MyVulkanManager::sqsCTL->pipelineLayout,
                              MyVulkanManager::sqsCTL->pipeline,
                              &(MyVulkanManager::sqsCTL->descSet[
                                  TextureManager::getVkDescriptorSetIndexForCommonTexLight(
                                      My3DLayer::qizi->texturename)]));
    MatrixState3D::popMatrix();
    for (int i = 0; i < QIZIGUN_NUMBER; i++)
    {
        MatrixState3D::pushMatrix();
        if (My3DLayer::flag[2] != 1)
        {
            if (i == 0)
                MatrixState3D::translate(My3DLayer::sp3Tree[2]->x, My3DLayer::qizi->y, My3DLayer::sp3Tree[2]->z);
            else
            {
                MatrixState3D::translate(My3DLayer::sp3Tree[2]->x, My3DLayer::qizi->y, My3DLayer::sp3Tree[2]->z);
                MatrixState3D::rotate(180.0, 0.0, 1.0, 0.0);
            }
        }
        else
        {
            if (i == 0)
            {
                MatrixState3D::translate(My3DLayer::sp3Tree[2]->x, My3DLayer::qizi->y, My3DLayer::sp3Tree[2]->z);
                MatrixState3D::rotate(90.0, 0.0, 1.0, 0.0);
            }
            else
            {
                MatrixState3D::translate(My3DLayer::sp3Tree[2]->x, My3DLayer::qizi->y, My3DLayer::sp3Tree[2]->z);
                MatrixState3D::rotate(-90.0, 0.0, 1.0, 0.0);
            }
        }
        MatrixState3D::scale(1.1, 1.1, 1.1);
        My3DLayer::qizigun[i]->drawSelf(MyVulkanManager::cmdBuffer, MyVulkanManager::sqsCTL->pipelineLayout,
                                        MyVulkanManager::sqsCTL->pipeline,
                                        &(MyVulkanManager::sqsCTL->descSet[
                                            TextureManager::getVkDescriptorSetIndexForCommonTexLight(
                                                My3DLayer::qizigun[i]->texturename)]));
        MatrixState3D::popMatrix();
    }

    for (int i = 0; i < TANSHE_NUMBER; i++)
    {
        MatrixState3D::pushMatrix();
        MatrixState3D::translate(My3DLayer::tanshe[i]->x, My3DLayer::tanshe[i]->y, My3DLayer::tanshe[i]->z);
        MatrixState3D::rotate(My3DLayer::tanshe[i]->Rangle, My3DLayer::tanshe[i]->Rx, My3DLayer::tanshe[i]->Ry,
                              My3DLayer::tanshe[i]->Rz);
        My3DLayer::tanshe[i]->drawSelf(MyVulkanManager::cmdBuffer, MyVulkanManager::sqsCTL->pipelineLayout,
                                       MyVulkanManager::sqsCTL->pipeline,
                                       &(MyVulkanManager::sqsCTL->descSet[
                                           TextureManager::getVkDescriptorSetIndexForCommonTexLight(
                                               My3DLayer::tanshe[i]->texturename)]));
        MatrixState3D::popMatrix();
    }

    ZFTManager::Update_mainbox();

    MatrixState3D::pushMatrix();
    MatrixState3D::translate(My3DLayer::mainBox->x, My3DLayer::mainBox->y, My3DLayer::mainBox->z);
    MatrixState3D::scale(ZFTManager::MainScalex, ZFTManager::MainScaley, ZFTManager::MainScalez);
    MatrixState3D::rotate(My3DLayer::mainBox->Rangle, My3DLayer::mainBox->Rx, My3DLayer::mainBox->Ry,
                          My3DLayer::mainBox->Rz);
    My3DLayer::mainBox->drawSelf(MyVulkanManager::cmdBuffer, MyVulkanManager::sqsCTL->pipelineLayout,
                                 MyVulkanManager::sqsCTL->pipeline,
                                 &(MyVulkanManager::sqsCTL->descSet[
                                     TextureManager::getVkDescriptorSetIndexForCommonTexLight(
                                         My3DLayer::mainBox->texturename)]));
    MatrixState3D::popMatrix();

    if (!TanShe::tanshe_flag)
    {
        DiCiBox::DiCiCheckLife();
        JiaZiBox::JiaZiCheckLife();
        DiaoLuoManager::DiaoLuoCheckLife();
        YiDongBox::YiDongCheckLife();
    }

    TanShe::TanSheCheck();
    ZFTManager::Win();

    if (!ZFTManager::life)
    {
        switch (ZFTManager::Diefoncution)
        {
        case 1:
            {
                ZFTManager::ZFTDiCiDie();
                break;
            }
        case 2:
            {
                ZFTManager::ZFTJiaZiDie();
                break;
            }
        case 3:
            {
                ZFTManager::ZFTDiaoLuoDie();
                break;
            }
        case 4:
            {
                ZFTManager::ZFTYiDongDie();
                break;
            }
        }
    }
    if (My3DLayer::mainBox->y <= -50)
    {
        for (int i = 0; i < ATTACH_BOX_NUMBER; i++)
        {
            My3DLayer::attachbox_draw_count[i] = 0;
        }
        for (int i = 0; i < ATTACH_BOX_NUMBER; i++)
        {
            My3DLayer::attachbox_draw_licence[i] = 0;
        }
    }
}

void MyDraw::Draw2Dobject()
{
    if (!Touch2D::Button_start)
    {
        MatrixState2D::pushMatrix();
        MatrixState2D::translate(0.05, 0.5, 0);
        MatrixState2D::scale(0.6, 0.25, 0);

        MyVulkanManager::d2dA->drawSelf(MyVulkanManager::cmdBuffer, MyVulkanManager::sqsD2D->pipelineLayout,
                                        MyVulkanManager::sqsD2D->pipeline,
                                        &(MyVulkanManager::sqsD2D->descSet[
                                            TextureManager::getVkDescriptorSetIndexForDashboard2D(steps_texture)]));

        MatrixState2D::popMatrix();
        MatrixState2D::pushMatrix();
        MatrixState2D::translate(0, 0.25, 0);
        MatrixState2D::scale(0.2, 0.15, 0);

        MyVulkanManager::d2dA->drawSelf(MyVulkanManager::cmdBuffer, MyVulkanManager::sqsD2D->pipelineLayout,
                                        MyVulkanManager::sqsD2D->pipeline,
                                        &(MyVulkanManager::sqsD2D->descSet[
                                            TextureManager::getVkDescriptorSetIndexForDashboard2D(
                                                dianjistart_texture)]));
        MatrixState2D::popMatrix();
        if (Touch2D::Button_sound)
        {
            MatrixState2D::pushMatrix();
            MatrixState2D::translate(0, -0.5, 0);
            MatrixState2D::scale(0.1, 0.1, 0);
            MyVulkanManager::d2dA->drawSelf(MyVulkanManager::cmdBuffer, MyVulkanManager::sqsD2D->pipelineLayout,
                                            MyVulkanManager::sqsD2D->pipeline,
                                            &(MyVulkanManager::sqsD2D->descSet[
                                                TextureManager::getVkDescriptorSetIndexForDashboard2D(
                                                    musicclosebtn_texture)]));
            MatrixState2D::popMatrix();
        }
        else
        {
            MatrixState2D::pushMatrix();
            MatrixState2D::translate(0, -0.5, 0);
            MatrixState2D::scale(0.1, 0.1, 0);
            MyVulkanManager::d2dA->drawSelf(MyVulkanManager::cmdBuffer, MyVulkanManager::sqsD2D->pipelineLayout,
                                            MyVulkanManager::sqsD2D->pipeline,
                                            &(MyVulkanManager::sqsD2D->descSet[
                                                TextureManager::getVkDescriptorSetIndexForDashboard2D(
                                                    musicbtn_texture)]));

            MatrixState2D::popMatrix();
        }
        MatrixState2D::pushMatrix();
        MatrixState2D::translate(-0.4, -0.5, 0);
        MatrixState2D::scale(0.1, 0.1, 0);
        MyVulkanManager::d2dA->drawSelf(MyVulkanManager::cmdBuffer, MyVulkanManager::sqsD2D->pipelineLayout,
                                        MyVulkanManager::sqsD2D->pipeline,
                                        &(MyVulkanManager::sqsD2D->descSet[
                                            TextureManager::getVkDescriptorSetIndexForDashboard2D(about_texture)]));
        MatrixState2D::popMatrix();

        if (!Touch2D::Button_exit)
        {
            MatrixState2D::pushMatrix();
            MatrixState2D::translate(0.4, -0.5, 0);
            MatrixState2D::scale(0.1, 0.1, 0);
            MyVulkanManager::d2dA->drawSelf(MyVulkanManager::cmdBuffer, MyVulkanManager::sqsD2D->pipelineLayout,
                                            MyVulkanManager::sqsD2D->pipeline,
                                            &(MyVulkanManager::sqsD2D->descSet[
                                                TextureManager::getVkDescriptorSetIndexForDashboard2D(
                                                    exitbtn_texture)]));
            MatrixState2D::popMatrix();
        }
    }

    if (!ZFTManager::life)
    {
        MatrixState2D::pushMatrix();
        MatrixState2D::translate(-0.11, -0.5, 0);
        MatrixState2D::scale(0.1, 0.1, 0);
        MyVulkanManager::d2dA->drawSelf(MyVulkanManager::cmdBuffer, MyVulkanManager::sqsD2D->pipelineLayout,
                                        MyVulkanManager::sqsD2D->pipeline,
                                        &(MyVulkanManager::sqsD2D->descSet[
                                            TextureManager::getVkDescriptorSetIndexForDashboard2D(resume_texture)]));
        MatrixState2D::popMatrix();

        MatrixState2D::pushMatrix();
        MatrixState2D::translate(0.11, -0.5, 0);
        MatrixState2D::scale(0.1, 0.1, 0);
        MyVulkanManager::d2dA->drawSelf(MyVulkanManager::cmdBuffer, MyVulkanManager::sqsD2D->pipelineLayout,
                                        MyVulkanManager::sqsD2D->pipeline,
                                        &(MyVulkanManager::sqsD2D->descSet[
                                            TextureManager::getVkDescriptorSetIndexForDashboard2D(back_texture)]));
        MatrixState2D::popMatrix();
    }

    if (Touch2D::Button_about)
    {
        MatrixState2D::pushMatrix();
        MatrixState2D::translate(0, -0.1, 0);
        MatrixState2D::scale(0.4, 0.2, 0);
        MyVulkanManager::d2dA->drawSelf(MyVulkanManager::cmdBuffer, MyVulkanManager::sqsD2D->pipelineLayout,
                                        MyVulkanManager::sqsD2D->pipeline,
                                        &(MyVulkanManager::sqsD2D->descSet[
                                            TextureManager::getVkDescriptorSetIndexForDashboard2D(guanyu_texture)]));
        MatrixState2D::popMatrix();
    }
}

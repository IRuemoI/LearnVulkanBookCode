#include "Touch2D.h"
#include "MyVulkanManager.h"
#include "AudioManager.h"
#include "My3DLayer.h"
#include "ZFTManager.h"
#include <stdio.h>

#define Touch_value 20
bool Touch2D::Button_about = false;
bool Touch2D::Button_resume = false;
bool Touch2D::Button_exit = false;
bool Touch2D::Button_start = false;
bool Touch2D::Button_sound = false;
bool Touch2D::Button_back = false;
bool Touch2D::isBGMusicOff = false;
bool Touch2D::isEffectMusicOff = false;

void Touch2D::CheckButton_about(int x, int y)
{
    if (y > (int)MyVulkanManager::screenHeight - 100 && y < (int)MyVulkanManager::screenHeight - 60 && x > (int)
        MyVulkanManager::screenWidth - 140 && x < (int)MyVulkanManager::screenWidth - 50)
    {
        Touch2D::Button_about = !Touch2D::Button_about;
    }
}

void Touch2D::CheckButton_sound(int x, int y)
{
    if (y > (int)MyVulkanManager::screenHeight - 100 && y < (int)MyVulkanManager::screenHeight - 60 && x > (int)
        MyVulkanManager::screenWidth - 140 && x < (int)MyVulkanManager::screenWidth - 50)
    {
        Touch2D::Button_sound = !Touch2D::Button_sound;
    }
}

void Touch2D::CheckButton_exit(int x, int y)
{
    if (y > (int)MyVulkanManager::screenHeight - 100 && y < (int)MyVulkanManager::screenHeight - 60 && x > (int)
        MyVulkanManager::screenWidth - 140 && x < (int)MyVulkanManager::screenWidth - 50)
    {
        printf("Exit button clicked\n");
        Touch2D::Button_exit = true;
    }
}

void Touch2D::CheckButton_start(int x, int y)
{
    if (y > (int)MyVulkanManager::screenHeight / 2 - 50 && y < (int)MyVulkanManager::screenHeight / 2 + 50 && x > (int)
        MyVulkanManager::screenWidth / 2 - 50 && x < (int)MyVulkanManager::screenWidth / 2 + 50)
    {
        Touch2D::Button_start = !Touch2D::Button_start;
        printf("Start button clicked\n");
    }
}

void Touch2D::CheckButton_resume(int x, int y)
{
    if (y > (int)MyVulkanManager::screenHeight / 2 - 50 && y < (int)MyVulkanManager::screenHeight / 2 + 50 && x > (int)
        MyVulkanManager::screenWidth / 2 - 50 && x < (int)MyVulkanManager::screenWidth / 2 + 50)
    {
        Touch2D::Button_resume = !Touch2D::Button_resume;
        printf("Resume button clicked\n");
    }
}

void Touch2D::CheckButton_back(int x, int y)
{
    if (y > (int)MyVulkanManager::screenHeight - 100 && y < (int)MyVulkanManager::screenHeight - 60 && x > (int)
        MyVulkanManager::screenWidth - 140 && x < (int)MyVulkanManager::screenWidth - 50)
    {
        printf("Back button clicked\n");
        My3DLayer::RepeatLevel();
    }
}

void Touch2D::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    printf("Mouse callback - button: %d, action: %d\n", button, action);
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        int x = (int)xpos;
        int y = (int)(MyVulkanManager::screenHeight - ypos);
        printf("Mouse clicked at: x=%d, y=%d (original: %.0f)\n", x, y, ypos);
        int startX1 = (int)MyVulkanManager::screenWidth / 2 - 50;
        int startX2 = (int)MyVulkanManager::screenWidth / 2 + 50;
        int startY1 = (int)MyVulkanManager::screenHeight / 2 - 50;
        int startY2 = (int)MyVulkanManager::screenHeight / 2 + 50;
        printf("Start button region: x[%d,%d], y[%d,%d]\n", startX1, startX2, startY1, startY2);

        if (ZFTManager::life && My3DLayer::BoxPos < 4)
        {
            if (!Touch2D::Button_start)
            {
                Touch2D::CheckButton_start(x, y);
                Touch2D::CheckButton_about(x, y);
                Touch2D::CheckButton_exit(x, y);
                Touch2D::CheckButton_sound(x, y);

                if (Touch2D::Button_start)
                {
                    AudioManager::playBGM("assets/sounds/bgm.wav", true);
                }
            }
            else
            {
                int goDir = My3DLayer::flagGo[My3DLayer::BoxPos];
                if (goDir == 0)
                {
                    My3DLayer::mainBox->Rz = 1;
                    My3DLayer::mainBox->Goorientation = 0;

                    if (!Touch2D::isEffectMusicOff)
                    {
                        AudioManager::playSound("assets/sounds/dingdong.wav");
                    }
                }
                else if (goDir == 1)
                {
                    My3DLayer::mainBox->Rx = 1;
                    My3DLayer::mainBox->Goorientation = 1;

                    if (!Touch2D::isEffectMusicOff)
                    {
                        AudioManager::playSound("assets/sounds/dingdong.wav");
                    }
                }
                else if (goDir == 2)
                {
                    My3DLayer::mainBox->Rz = 1;
                    My3DLayer::mainBox->Goorientation = 2;

                    if (!Touch2D::isEffectMusicOff)
                    {
                        AudioManager::playSound("assets/sounds/dingdong.wav");
                    }
                }
            }
        }
        else
        {
            if (!ZFTManager::life)
            {
                Touch2D::CheckButton_resume(x, y);
                Touch2D::CheckButton_back(x, y);
                if (Touch2D::Button_resume || Touch2D::Button_back)
                {
                    if (Touch2D::Button_resume)
                    {
                        printf("Resume button clicked\n");
                        My3DLayer::RepeatLevel();
                    }
                    else if (Touch2D::Button_back)
                    {
                        printf("Back button clicked\n");
                        AudioManager::stopBGM();
                        Touch2D::Button_back = false;
                    }
                }
            }
            else if (My3DLayer::BoxPos == 4)
            {
                My3DLayer::NextLevel();
            }
        }
    }
}

void Touch2D::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
}

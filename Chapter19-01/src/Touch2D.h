#ifndef VULKANEXBASE_SQUARE_TOUCH2D_H
#define VULKANEXBASE_SQUARE_TOUCH2D_H

#include <string>

using namespace std;

struct GLFWwindow;

class Touch2D
{
public:
    static bool Button_about;
    static bool Button_resume;
    static bool Button_exit;
    static bool Button_start;
    static bool Button_sound;
    static bool Button_back;

    static bool isBGMusicOff;
    static bool isEffectMusicOff;

    static void CheckButton_about(int x, int y);
    static void CheckButton_resume(int x, int y);
    static void CheckButton_exit(int x, int y);
    static void CheckButton_start(int x, int y);
    static void CheckButton_sound(int x, int y);
    static void CheckButton_back(int x, int y);

    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

    static bool flag;
};

#endif

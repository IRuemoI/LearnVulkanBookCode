#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H

#include <string>

class AudioManager
{
public:
    static void init();
    static void cleanup();

    static void playBGM(const char* path, bool loop = true);
    static void playSound(const char* path);

    static void stopBGM();

    static void setBGMVolume(int volume); // 0-128
    static void setSoundVolume(int volume); // 0-128

    static bool isBGMMuted;
    static bool isSoundMuted;
};

#endif

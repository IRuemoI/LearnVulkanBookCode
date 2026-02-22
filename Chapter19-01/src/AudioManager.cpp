#include "AudioManager.h"
#include <SDL_mixer.h>
#include <stdio.h>

bool AudioManager::isBGMMuted = false;
bool AudioManager::isSoundMuted = false;

static Mix_Music* g_bgm = nullptr;

void AudioManager::init()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }
}

void AudioManager::cleanup()
{
    stopBGM();
    Mix_CloseAudio();
}

void AudioManager::playBGM(const char* path, bool loop)
{
    if (isBGMMuted) return;

    if (g_bgm)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(g_bgm);
        g_bgm = nullptr;
    }

    g_bgm = Mix_LoadMUS(path);
    if (g_bgm == nullptr)
    {
        printf("Failed to load BGM %s! SDL_mixer Error: %s\n", path, Mix_GetError());
        return;
    }

    if (Mix_PlayMusic(g_bgm, loop ? -1 : 0) == -1)
    {
        printf("Failed to play BGM! SDL_mixer Error: %s\n", Mix_GetError());
    }
}

void AudioManager::playSound(const char* path)
{
    if (isSoundMuted) return;

    Mix_Chunk* chunk = Mix_LoadWAV(path);
    if (chunk == nullptr)
    {
        printf("Failed to load sound %s! SDL_mixer Error: %s\n", path, Mix_GetError());
        return;
    }

    Mix_PlayChannel(-1, chunk, 0);
    Mix_FreeChunk(chunk);
}

void AudioManager::stopBGM()
{
    if (g_bgm)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(g_bgm);
        g_bgm = nullptr;
    }
}

void AudioManager::setBGMVolume(int volume)
{
    if (volume < 0) volume = 0;
    if (volume > 128) volume = 128;
    Mix_VolumeMusic(volume);
}

void AudioManager::setSoundVolume(int volume)
{
    if (volume < 0) volume = 0;
    if (volume > 128) volume = 128;
    Mix_Volume(-1, volume);
}

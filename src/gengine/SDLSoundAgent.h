#ifndef HEADER_SDLSOUNDAGENT_H
#define HEADER_SDLSOUNDAGENT_H

#include "SoundAgent.h"
#include "SDLMusicLooper.h"

#include "SDL.h"
#include "SDL_mixer.h"
#include <string>

/**
 * Sound and music.
 */
class SDLSoundAgent : public SoundAgent {
    private:
    static BaseMsg *ms_finished;
    Mix_Music *m_music;
    SDLMusicLooper *m_looper;
    std::string m_playingPath;
    int m_soundVolume;
    int m_musicVolume;

    private:
        std::string generateIdName(const Path &file);
        Mix_Chunk *findChunk(const std::string &name);

        static void musicFinished();
    protected:
        virtual void own_init();
        virtual void own_shutdown();
        virtual void reinit();

        virtual void setSoundVolume(int volume);
        virtual void setMusicVolume(int volume);
    public:
        virtual int playSound(Mix_Chunk *sound, int volume, int loops=0);

        virtual void playMusic(const Path &file,
                BaseMsg *finished);
        virtual void stopMusic();
};

#endif

#ifndef HEADER_SOUNDAGENT_H
#define HEADER_SOUNDAGENT_H

class Path;
class BaseMsg;

#include "BaseAgent.h"
#include "Name.h"
#include "IntMsg.h"

#include "SDL.h"
#include "SDL_mixer.h"
#include <string>

/**
 * Sound and music interface.
 */
class SoundAgent : public BaseAgent {
    AGENT(SoundAgent, Name::SOUND_NAME);
    protected:
        virtual void own_init();
        virtual void setSoundVolume(int volume) = 0;
        virtual void setMusicVolume(int volume) = 0;
    public:
        virtual void reinit();
        virtual int playSound(Mix_Chunk *sound, int volume,
                int loops=0) = 0;

        virtual void playMusic(const Path &file,
                BaseMsg *finished) = 0;
        virtual void stopMusic() = 0;
        void receiveString(const StringMsg *msg);
};

#endif

#ifndef HEADER_SOUNDAGENT_H
#define HEADER_SOUNDAGENT_H

class BaseMsg;

#include "BaseAgent.h"
#include "Name.h"
#include "IntMsg.h"
#include "Path.h"

#include "SDL.h"
#include "SDL_mixer.h"
#include <string>

/**
 * Sound and music interface.
 */
class SoundAgent : public BaseAgent {
    AGENT(SoundAgent, Name::SOUND_NAME);
    public:
        virtual int playSound(Mix_Chunk *sound) = 0;
        virtual void setSoundVolume(int volume) = 0;
        virtual int getSoundVolume() = 0;

        virtual void playMusic(const Path &file,
                BaseMsg *finished) = 0;
        virtual void setMusicVolume(int volume) = 0;
        virtual int getMusicVolume() = 0;
        virtual void stopMusic() = 0;

        virtual void receiveInt(const IntMsg *msg);
};

#endif

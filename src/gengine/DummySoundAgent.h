#ifndef HEADER_DUMMYSOUNDAGENT_H
#define HEADER_DUMMYSOUNDAGENT_H

#include "SoundAgent.h"

/**
 * NO sound and music.
 */
class DummySoundAgent : public SoundAgent {
    public:
        virtual int playSound(Mix_Chunk *, int /*volume*/, int /*loops*/)
        { return -1; }
        virtual void setSoundVolume(int ) {}
        virtual int getSoundVolume() { return 0; }

        virtual void playMusic(const Path &,
                BaseMsg *finished)
        {
            if (finished) {
                delete finished;
            }
        }
        virtual void setMusicVolume(int ) {}
        virtual int getMusicVolume() { return 0; }
        virtual void stopMusic() {}
};

#endif

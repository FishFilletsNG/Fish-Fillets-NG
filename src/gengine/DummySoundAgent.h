#ifndef HEADER_DUMMYSOUNDAGENT_H
#define HEADER_DUMMYSOUNDAGENT_H

#include "SoundAgent.h"

/**
 * NO sound and music.
 */
class DummySoundAgent : public SoundAgent {
    protected:
        virtual void setSoundVolume(int ) {}
        virtual void setMusicVolume(int ) {}
    public:
        virtual int playSound(Mix_Chunk *, int /*volume*/, int /*loops*/)
        { return -1; }

        virtual void playMusic(const Path &,
                BaseMsg *finished)
        {
            if (finished) {
                delete finished;
            }
        }
        virtual void stopMusic() {}
};

#endif

#ifndef HEADER_SDLMUSICLOOPER_H
#define HEADER_SDLMUSICLOOPER_H

class Path;

#include "SDL.h"
#include "SDL_mixer.h"

/**
 * Playing music in a loop.
 */
class SDLMusicLooper {
    private:
    int m_volume;
    int m_position;

    Mix_Chunk *m_music;
    int m_startLoop, m_endLoop;

    private:
        void lookupLoopData(const Path &file, int multiplier);
        static void musicOutput(void *udata, Uint8 *stream, int length);

    public:
        SDLMusicLooper(const Path &file);
        virtual ~SDLMusicLooper();
        void setVolume(int volume);
        void start();
        void stop();
};


#endif

#ifndef HEADER_RESSOUNDPACK_H
#define HEADER_RESSOUNDPACK_H

class Path;

#include "ResourcePack.h"

#include "SDL_mixer.h"

/**
 * Sound resources.
 */
class ResSoundPack : public ResourcePack<Mix_Chunk*> {
    public:
        virtual const char *getName() const { return "sound_pack"; }

        static Mix_Chunk *loadSound(const Path &file);
        void addSound(const std::string &name, const Path &file);
        virtual void unloadRes(Mix_Chunk *res);
};

#endif


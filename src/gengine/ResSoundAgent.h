#ifndef HEADER_RESSOUNDAGENT_H
#define HEADER_RESSOUNDAGENT_H

#include "BaseAgent.h"
#include "ResourcePack.h"
#include "Name.h"
#include "Path.h"

#include "SDL_mixer.h"

/**
 * Sound resources.
 */
class ResSoundAgent : public BaseAgent, public ResourcePack<Mix_Chunk*> {
    AGENT(ResSoundAgent, Name::RESSOUND_NAME);
    protected:
    virtual void unloadRes(Mix_Chunk *res);
    virtual void own_shutdown();
    public:
    Mix_Chunk *loadSound(const Path &file);
    void addSound(const std::string &name, const Path &file);
};

#endif


#ifndef HEADER_RESSOUNDAGENT_H
#define HEADER_RESSOUNDAGENT_H

#include "ResourceAgent.h"
#include "Name.h"
#include "Path.h"

#include "SDL_mixer.h"

/**
 * Sound resources.
 */
class ResSoundAgent : public ResourceAgent<Mix_Chunk*> {
    AGENT(ResSoundAgent, Name::RESSOUND_NAME);
    protected:
    virtual void unloadRes(Mix_Chunk *res);
    public:
    void addSound(const std::string &name, const Path &file);

    //FIXME: stop music before shutdown?
};

#endif

